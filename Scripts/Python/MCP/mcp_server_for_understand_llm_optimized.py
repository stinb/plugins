import argparse
import base64
import json
import os
import re
import sys
from typing import Annotated, List, Literal, Optional, Union
from pydantic import Field

# Add DLL directory if specified in environment variable (for Windows)
if "UNDERSTAND_DLL_DIR" in os.environ:
    os.add_dll_directory(os.environ["UNDERSTAND_DLL_DIR"])

import understand
from mcp.server.fastmcp import FastMCP

# Define MCP server instance
mcp = FastMCP("UNDERSTAND_MCP")

# Global variable to store the opened database
db = None

def require_db() -> understand.Db:
    """Ensure the database is open and return it."""
    if db is None:
        raise RuntimeError("Database is not open")
    return db

def get_filterable_kind_name(ent) -> str:
    """
    Get an entity kind name that works correctly as a filter string.

    Uses the short name (kind().name()) if it matches via check(), otherwise
    falls back to the longname which always works. This handles edge cases
    where the short name doesn't match due to naming inconsistencies in the
    Understand API (e.g., "Template Parameter" vs "TemplateParameter").
    """
    kind = ent.kind()
    short_name = kind.name()
    if kind.check(short_name):
        return short_name
    return kind.longname()

def encode_cursor(offset: int) -> str:
    """Encode pagination offset as a cursor string."""
    return base64.b64encode(json.dumps({"offset": offset}).encode()).decode()

def decode_cursor(cursor: str) -> int:
    """Decode a cursor string to get the offset. Returns 0 if invalid."""
    try:
        data = json.loads(base64.b64decode(cursor).decode())
        return data.get("offset", 0)
    except:
        return 0

def _pick_sample_ref_for_entity(ent):
    """
    Pick a sample reference for an entity: first definein, else first declarein, else first ref with file.
    Returns (ref, file_ent, kind_label) or (None, None, None).
    """
    refs = list(ent.refs())
    definein = [r for r in refs if r.kind().check("definein")]

    if definein:
        r = definein[0]
        return r, r.file(), "definein"
    declarein = [r for r in refs if r.kind().check("declarein")]
    if declarein:
        r = declarein[0]
        return r, r.file(), "declarein"
    if refs:
        r = refs[0]
        return r, r.file(), r.kind().longname()
    return None, None, None


def _build_no_source_error(ent_id: int, ent) -> str:
    """Build error message when entity has no source, with workflow suggestion and optional 5-line example."""
    ent_name = ent.name()
    ent_kind = ent.kind().name()
    base = (
        f"Entity {ent_id} ('{ent_name}', {ent_kind}) has no source code. "
        "Retrieve references with get_entity_references(ent_id={ent_id}), "
        "then get source at those locations with get_entity_source(ent_id=<file_id>, start_line=<line>-2, end_line=<line>+2)."
    ).format(ent_id=ent_id)

    sample_ref, file_ent, kind_label = _pick_sample_ref_for_entity(ent)
    if not sample_ref or not file_ent:
        return base

    line = sample_ref.line()
    file_id = file_ent.id()
    file_contents = file_ent.contents()
    if not file_contents:
        return base

    lines = file_contents.split("\n")
    total = len(lines)
    start_1 = max(1, line - 2)
    end_1 = min(total, line + 2)
    snippet = "\n".join(lines[start_1 - 1 : end_1])

    example = (
        f'\n\nExample: retrieve five lines for the first "{kind_label}" ref at file_id={file_id}, line={line}:\n'
        f"  get_entity_source(ent_id={file_id}, start_line={start_1}, end_line={end_1})\n"
        "returns (5-line window):\n"
        "---\n"
        f"{snippet}\n"
        "---"
    )
    return base + example


def _key_ent_file_stem(ent) -> str:
    name = (ent.name() or "").strip()
    if "." in name:
        return name.rsplit(".", 1)[0].lower()
    return name.lower()


def _key_ent_incoming_depends_count(entities_list: list) -> dict:
    """For each entity, count how many others in the list depend on it (via depends())."""
    entity_set = set(entities_list)
    incoming = {}
    for ent in entity_set:
        try:
            for to_ent in ent.depends().keys():
                if to_ent in entity_set and to_ent is not ent:
                    incoming[to_ent] = incoming.get(to_ent, 0) + 1
        except Exception:
            pass
    return incoming


def _key_ent_group_by_stem(arch, recursive: bool = False) -> dict:
    """Group entities by stem (name without extension). recursive=False uses only direct entities."""
    groups = {}
    try:
        for e in arch.ents(recursive):
            try:
                stem = _key_ent_file_stem(e)
                if not stem:
                    continue
                groups.setdefault(stem, []).append(e)
            except Exception:
                pass
    except Exception:
        pass
    return groups


def _key_entities_ranked_header(arch, recursive: bool = False) -> list:
    """Rank entities: preferred (header file kind) from each stem first, then rest. Stems ordered by exact name match
    to arch, then sum incoming dependency count, then ascending CountLineCode. Returns list of entities."""
    groups = _key_ent_group_by_stem(arch, recursive=recursive)
    if not groups:
        return []
    all_entities = [e for entities in groups.values() for e in entities]
    incoming_count = _key_ent_incoming_depends_count(all_entities)
    arch_lower = (arch.name() or "").lower()

    def lines(ent):
        try:
            return ent.metric("CountLineCode") or 0
        except Exception:
            return 0

    def chosen_lines_for_stem(entities):
        preferred = [e for e in entities if e.kind().check("header file")]
        candidates = preferred if preferred else entities
        return min(lines(e) for e in candidates)

    def sort_key(item):
        name, entities = item
        exact_match = 1 if name == arch_lower else 0
        sum_inc = sum(incoming_count.get(e, 0) for e in entities)
        chosen_lines = chosen_lines_for_stem(entities)
        return (exact_match, sum_inc, -chosen_lines, name)

    sorted_stems = sorted(groups.items(), key=sort_key, reverse=True)
    selected = []
    unselected = []
    for _stem, entities in sorted_stems:
        preferred = [e for e in entities if e.kind().check("header file")]
        chosen = min(preferred, key=lines) if preferred else min(entities, key=lines)
        for e in entities:
            if e is chosen:
                selected.append(e)
            else:
                unselected.append(e)
    return selected + unselected


def _arch_has_direct_non_unparsed_entities(arch) -> bool:
    """True if this architecture has at least one direct entity that is not unparsed."""
    for ent in arch.ents(False):
        try:
            if not ent.kind().check("unparsed"):
                return True
        except Exception:
            continue
    return False


def _get_architecture_children_bfs(arch, max_results: int) -> tuple:
    """Return a limited list of descendant architectures (children, grandchildren, ...) via BFS.
    Prefer nodes that have direct non-unparsed entities (like get_top_level_dirs2). Each node
    appears at most once. May return more than max_results when a single node has many children
    (see lines 226-234). Returns (list of understand.Arch, depth_reached: int)."""
    if max_results <= 0:
        return []
    initial_arch = arch
    top = []
    current_level = []
    next_level = [arch]
    depth = -1  # So it goes to 0 on first iteration when "arch" becomes current_level/top.
    while (next_level and (len(top) + len(next_level)) < max_results):
        depth += 1
        to_visit = next_level
        current_level = []
        next_level = []
        for arch in to_visit:
            if _arch_has_direct_non_unparsed_entities(arch):
                top.append(arch)
            else:
                current_level.append(arch)
            next_level.extend(list(arch.children()))
    top.extend(current_level)
    if len(top) == 1 and top[0].children():
        # If there's only one arch in the list because it has more children
        # than max results, return all it's children and limit to max results
        # in the caller based on the sorting metric.
        top.extend(list(top[0].children()))
        depth += 1
        if not _arch_has_direct_non_unparsed_entities(top[0]):
            top.pop(0)
    if initial_arch in top and (len(top) == 1 or not _arch_has_direct_non_unparsed_entities(initial_arch)):
        # Don't return the initial arch as a child unless it has direct entities and
        # there are other children reported
        top.remove(initial_arch)
    return top, depth


def paginate_results(items: list, max_results: int, cursor: Optional[str]) -> tuple:
    """
    Apply pagination to a list of items.
    Returns: (paginated_items, total_count, truncated, next_cursor_or_none)
    """
    offset = decode_cursor(cursor) if cursor else 0
    total_count = len(items)

    # Slice from offset
    paginated = items[offset:offset + max_results]

    # Check if there are more results
    next_offset = offset + len(paginated)
    truncated = next_offset < total_count
    next_cursor = encode_cursor(next_offset) if truncated else None

    return paginated, total_count, truncated, next_cursor

def filter_entities_by_name_pattern(entities, name_pattern: Optional[str]):
    """
    Filter entities by name pattern (simple substring matching, case-insensitive).

    Args:
        entities: List, set, or iterator of entities
        name_pattern: Substring to search for (case-insensitive). If None, returns all entities.

    Returns:
        Filtered list of entities
    """
    if not name_pattern:
        return list(entities) if not isinstance(entities, list) else entities

    pattern_lower = name_pattern.lower()
    if isinstance(entities, (set, tuple)):
        return [ent for ent in entities if pattern_lower in ent.name().lower()]
    else:
        # Handle both lists and iterators
        return [ent for ent in entities if pattern_lower in ent.name().lower()]

def expand_containers_in_entity(ent: understand.Ent, kindstring: Optional[str] = None) -> set:
    """
    Expand containers in an entity to find entities matching kindstring.

    This function finds entities that are logically within an entity by expanding container
    entities to find nested entities. Uses a generic approach that works for any container type:
    - Files: use filerefs to find entities defined within the file
    - Python packages: use "python contain" refs to find files in the package
    - Generic containers (classes, functions, enums, etc.): use "define, declare" refs to find
      nested entities (methods, parameters, enumerators, etc.)

    Args:
        ent: The entity to search within (file, class, function, enum, package, etc.)
        kindstring: Optional entity kind to find (e.g., 'Function', 'Class', 'Parameter', 'Enumerator').
                   If None or empty, returns the entity itself and expands containers to find all children.

    Returns:
        Set of entities matching the kindstring (or all children if kindstring is None),
        including those found through container expansion
    """
    matching_entities = set()

    # Determine what type of container this is and expand accordingly
    files_to_expand = set()
    packages_to_expand = set()
    generic_containers_to_expand = set()

    # If no kindstring provided, treat the entity itself as matching and expand containers
    # Check if the entity itself matches
    if not kindstring or ent.kind().check(kindstring):
        matching_entities.add(ent)

    # Check if this entity is a container type
    # Files use filerefs (special case)
    if ent.kind().check("file ~unresolved ~unknown"):
        files_to_expand.add(ent)
    # Python packages use "python contain" refs (special case)
    elif ent.kind().check("python package ~unknown"):
        packages_to_expand.add(ent)
    # Generic containers: classes, functions, enums, etc. that contain entities via "define, declare" refs
    # Check if entity has any "define, declare" references (indicates it's a container)
    else:
        # Check if this entity has "define, declare" references (generic container detection)
        # This works for classes, functions, enums, and any other container type
        has_define_declare_refs = len(ent.refs("define, declare", "", True)) > 0
        if has_define_declare_refs:
            generic_containers_to_expand.add(ent)

    # Expand Python packages to get their files
    for pkg in packages_to_expand:
        for ref in pkg.refs("python contain", "file ~unknown ~unresolved", True):
            file_ent = ref.ent()
            if not kindstring or file_ent.kind().check(kindstring):
                # If no kind string, only expand direct children which are the files in this case
                matching_entities.add(file_ent)
            else:
                files_to_expand.add(file_ent)

    # Expand files to find nested entities (use filerefs for files)
    # Use "define, separate, ada declare body, vhdl declare" - "separate" is for Ada separate units,
    # not "declare" which would include entities merely declared in a file
    for file_ent in files_to_expand:
        # Kind string is allowed to be None or "" and will match everything in that case
        for ref in file_ent.filerefs("define, separate, ada declare body, vhdl declare", kindstring, True):
            matching_entities.add(ref.ent())

    # Expand generic containers (classes, functions, enums, etc.) via "define, declare" refs
    # This generic approach works for any entity type that contains other entities via references
    for container_ent in generic_containers_to_expand:
        # Kind string is allowed to be None or "" and will match everything in that case
        for ref in container_ent.refs("define, declare", kindstring, True):
            matching_entities.add(ref.ent())

    return matching_entities

def architecture_not_found_error(database: understand.Db, arch_name: str) -> str:
    """
    Return an enhanced error message when an architecture is not found.
    Works for any architecture type (Directory Structure, Cycles, Class Namespace, etc.).
    When the path contains '/', suggests known children of the parent path.
    """
    error_msg = f"Architecture '{arch_name}' not found. Use list_architectures_summary to discover available architectures."
    if "/" in arch_name:
        parent_path = "/".join(arch_name.split("/")[:-1])
        parent_arch = database.lookup_arch(parent_path)
        if parent_arch:
            children = parent_arch.children()
            if children:
                child_names = [c.name() for c in children[:5]]
                error_msg += f" Known children of '{parent_path}': {', '.join(child_names)}"
    return error_msg

def get_entities_by_scope(
    database: understand.Db,
    ent_id: Optional[int] = None,
    arch_name: Optional[str] = None,
    kindstring: Optional[str] = None,
) -> tuple[List, Optional[str]]:
    """
    Get entities based on entity scope, architecture scope, or both.

    This is a common function used by list_entities and find_entities_by_metric
    to handle entity/architecture expansion logic in one place.

    Args:
        database: The Understand database
        ent_id: Optional entity ID to search within
        arch_name: Optional architecture longname to filter by
        kindstring: Optional entity kind filter

    Returns:
        Tuple of (entities_list, error_message). If error_message is not None, entities_list will be empty.
    """
    entities = []

    if ent_id:
        # Entity-scoped (or entity + architecture)
        ent = database.ent_from_id(ent_id)
        if ent is None:
            return [], f"Entity with id {ent_id} not found."

        # Expand containers to find nested entities (kindstring can be None)
        matching_entities = expand_containers_in_entity(ent, kindstring)
        entities = list(matching_entities)

    if arch_name:
        # Architecture-scoped (or architecture + entity)
        arch = database.lookup_arch(arch_name)
        if arch is None:
            return [], architecture_not_found_error(database, arch_name)

        if ent_id:
            # Both ent_id and arch_name: filter entities from ent_id by architecture
            # Use the same logic as get_architecture_intersection
            filtered_entities = []
            for ent in entities:
                # Get all architectures this entity belongs to (with implicit=True for container expansion)
                entity_archs = database.archs(ent, implicit=True)

                # Check if entity belongs to the specified architecture
                belongs_to_arch = False
                for arch_ent in entity_archs:
                    arch_longname = arch_ent.longname()
                    if arch_longname == arch_name or arch_longname.startswith(arch_name + "/"):
                        belongs_to_arch = True
                        break

                if belongs_to_arch:
                    filtered_entities.append(ent)
            entities = filtered_entities
        else:
            # Architecture-scoped only
            if kindstring:
                # Expand containers to find nested entities (recursive=True to search child architectures for Directory Structure)
                matching_entities = expand_containers_in_architecture(arch, kindstring, True)
                entities = list(matching_entities)
            else:
                # Direct members only (no expansion)
                entities = list(arch.ents(False))

    if not ent_id and not arch_name:
        # Project-wide
        if kindstring:
            entities = database.ents(kindstring)
        else:
            entities = database.ents()

    return entities, None

def expand_containers_in_architecture(arch: understand.Arch, kindstring: str, recursive: bool) -> set:
    """
    Expand containers (files, classes, packages) in an architecture to find entities matching kindstring.

    This function finds entities that are logically within an architecture by expanding container
    entities to find nested entities. When recursive=True, arch.ents(recursive) already includes
    entities from child architectures, so this function expands containers within those entities.

    Args:
        arch: The architecture to search within
        kindstring: Entity kind to find (e.g., 'Function', 'Class', 'Method')
        recursive: If True, arch.ents(recursive) includes entities from child architectures

    Returns:
        Set of entities matching the kindstring, including those found through container expansion
    """
    matching_entities = set()

    # Get direct entities from architecture
    direct_entities = arch.ents(recursive)

    # Use expand_containers_in_entity for each entity to avoid code duplication
    for ent in direct_entities:
        # expand_containers_in_entity handles matching entities, files, classes, and packages
        expanded = expand_containers_in_entity(ent, kindstring)
        matching_entities.update(expanded)

    return matching_entities

@mcp.tool(name="lookup_entity_id")
def lookup_entity_id(
    name: Annotated[str, Field(description="The name or regex pattern to search for in the Understand database.")],
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string to narrow the search (e.g., 'Function', 'File', 'Class').")] = None,
    max_results: Annotated[int, Field(description="Maximum number of results to return. Default is 10 to keep responses concise.", ge=1, le=50)] = 10,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    Find entities by name (starting point for most workflows).

    Returns a concise list of matching entities with id, name, and kind fields.
    Use this to discover entities when you know their name but need the entity ID for other operations.

    Common workflow:
    1. lookup_entity_id(name="egrep.c", kindstring="File") → get entity ID (e.g., 276)
    2. Review matches and select the correct entity ID
    3. Optionally:get_entity_source(ent_id=276, start_line=80, end_line=82) → get source code
    4. Optionally: get_entity_details(ent_id=276) → get full details including location
    5. Optionally: get_entity_references_summary(ent_id=276) → explore usage

    Example: To read line 81 of "egrep.c":
    - Step 1: lookup_entity_id(name="egrep.c", kindstring="File") → returns ent_id=276
    - Step 2: get_entity_source(ent_id=276, start_line=81, end_line=81) → returns line 81

    The name parameter supports regex patterns for flexible searching.
    Supports pagination: use next_cursor from response to get more results.

    Kind filter syntax (applies to all kindstring/refkindstring parameters):
    - OR (comma): "function,class" matches functions OR classes
    - AND (space): "public function" matches public functions
    - NOT (tilde): "function ~unresolved" matches defined functions only
    - Precedence: OR binds tighter than AND, so "function,method ~unresolved" matches
      (all functions) OR (methods that are not unresolved)
    """
    entities = require_db().lookup(name, kindstring)
    if not entities:
        return {"entities": [], "total_count": 0, "truncated": False}

    # Apply pagination
    paginated, total_count, truncated, next_cursor = paginate_results(entities, max_results, cursor)

    # Return minimal but useful information: ID, name, and kind
    results = []
    for ent in paginated:
        results.append({
            "id": ent.id(),
            "name": ent.name(),
            "kind": ent.kind().name(),
        })

    response = {
        "entities": results,
        "total_count": total_count,
        "truncated": truncated,
    }
    if next_cursor:
        response["next_cursor"] = next_cursor
    return response

@mcp.tool(name="get_entity_details")
def get_entity_details(
    ent_id: Annotated[int, Field(description="The entity ID to get full details for. IMPORTANT: Entity IDs are arbitrary numbers assigned by Understand and cannot be guessed. You must discover the correct entity ID first using lookup_entity_id(name='...') or list_entities(...). Do not assume entity IDs are sequential or reuse IDs from unrelated entities.", ge=1)],
) -> Optional[dict]:
    """
    Get complete details about an entity including location and relationships.

    Returns: longname, kind, definition location (file path, line, column), type information,
    parent entity (if nested), and parameters (for functions/methods).

    Common workflow:
    1. lookup_entity_id(name="function_name", kindstring="Function") → get entity ID
    2. get_entity_details(ent_id=<id>) → get full details including location

    Use this after lookup_entity_id when you need to:
    - Identify which entity to work with when multiple matches exist
    - Get the file path and line number where an entity is defined
    - Understand entity relationships (parent/child)
    - Distinguish between similar entities (e.g., function overloads via parameters)

    The definition location helps match entities to files the user might have open.
    """
    ent = require_db().ent_from_id(ent_id)
    if ent is None:
        return None

    result = {
        "longname": ent.longname(),
        "kind": ent.kind().longname(),
    }

    # Get definition reference - this is the most useful for identifying where an entity is defined
    def_ref = ent.ref("definein")
    if not def_ref:
        # Fallback to declarein if no definein reference exists
        def_ref = ent.ref("declarein")

    if def_ref:
        file_ent = def_ref.file()
        if file_ent:
            # Use relname (relative path) if available, otherwise longname (full path)
            file_path = file_ent.relname() if file_ent.relname() else file_ent.longname()
            result["definition"] = {
                "file": file_path,
                "line": def_ref.line(),
                "column": def_ref.column(),
            }

    # Add type information if available (useful for variables, functions, etc.)
    ent_type = ent.type()
    if ent_type:
        result["type"] = ent_type

    # Add parent information if available (helps identify nested entities)
    parent = ent.parent()
    if parent:
        result["parent"] = {
            "id": parent.id(),
            "name": parent.name(),
            "kind": parent.kind().name(),
        }

    # Add parameters for functions/methods (helps distinguish overloads)
    params = ent.parameters()
    if params:
        result["parameters"] = params

    return result

@mcp.tool(name="get_entity_source")
def get_entity_source(
    ent_id: Annotated[int, Field(description="The entity ID to get source code for. IMPORTANT: Entity IDs are arbitrary numbers assigned by Understand and cannot be guessed. You must discover the correct entity ID first using lookup_entity_id(name='...') or list_entities(kindstring='File', name_pattern='...'). Do not assume entity IDs are sequential (e.g., 1, 2, 3) or reuse IDs from unrelated entities.", ge=1)],
    start_line: Annotated[Optional[Union[int, str]], Field(description="Optional start line number (1-based, relative to the entity's content). Line 1 is the first line of the entity. If specified, only returns code from this line onwards. Accepts numeric or string values.")] = None,
    end_line: Annotated[Optional[Union[int, str]], Field(description="Optional end line number (1-based, relative to the entity's content). If specified with start_line, returns only the specified line range. Accepts numeric or string values.")] = None,
    max_length: Annotated[int, Field(description="Maximum length of source code to return in characters. Default is 5000 to avoid filling the context window. Response will be truncated if longer.", ge=100, le=50000)] = 5000,
) -> dict:
    """
    Get source code for an entity with optional line range filtering.

    Returns: source code, truncated flag, line range returned, and total lines.

    Common workflow:
    1. lookup_entity_id(name="egrep.c", kindstring="File") → get entity ID (e.g., 276)
    2. get_entity_source(ent_id=276, start_line=80, end_line=82) → get source code

    It's safe to call this on any entity. If the entity has no source (e.g. parameters,
    namespaces, unresolved functions), the error message will include a code snippet from
    a reference location and guidance.

    Use this to:
    - Read function/method/class implementations
    - Get specific line ranges (e.g., lines 10-50) to focus on relevant code
    - Read file contents (for file entities)

    Supports line range filtering to avoid fetching large files. Response is automatically
    truncated if it exceeds max_length to preserve context window space.

    Tip: Use start_line and end_line parameters to get only the code section you need.
    """
    # Coerce parameters to correct types (handle string inputs from LLMs)
    if start_line is not None:
        start_line = int(start_line)
    if end_line is not None:
        end_line = int(end_line)

    ent = require_db().ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")

    # Use contents() directly - it's more efficient than lexer for getting text
    source_code = ent.contents()

    # Check if entity has source code (contents() returns empty string if not available)
    if not source_code:
        raise ValueError(_build_no_source_error(ent_id, ent))

    # Handle line range filtering
    lines = source_code.split('\n')
    total_lines = len(lines)

    # Determine line range
    if start_line is None:
        actual_start_line = 1
    else:
        actual_start_line = max(1, min(start_line, total_lines))

    if end_line is None:
        actual_end_line = total_lines
    else:
        actual_end_line = max(actual_start_line, min(end_line, total_lines))

    # Extract the line range (convert to 0-based index)
    selected_lines = lines[actual_start_line - 1:actual_end_line]
    source_code = '\n'.join(selected_lines)

    # Truncate if too long
    truncated = False
    if len(source_code) > max_length:
        source_code = source_code[:max_length]
        truncated = True
        # Calculate actual end line after truncation
        # Count complete lines only (number of newlines = complete lines when truncated mid-line)
        complete_line_count = source_code.count('\n')
        if complete_line_count > 0:
            # Truncate to last complete line for clean continuation
            last_newline = source_code.rfind('\n')
            source_code = source_code[:last_newline]
            actual_end_line = actual_start_line + complete_line_count - 1
        else:
            # Truncated within the first line - keep partial, report line 0 to signal re-fetch needed
            actual_end_line = actual_start_line - 1

    return {
        "source": source_code,
        "truncated": truncated,
        "start_line": actual_start_line,
        "end_line": actual_end_line,
        "total_lines": total_lines,
    }

@mcp.tool(name="get_entity_references_summary")
def get_entity_references_summary(
    ent_id: Annotated[int, Field(description="The entity ID to get a summary of references for. IMPORTANT: Entity IDs are arbitrary numbers assigned by Understand. Use lookup_entity_id(name='...') first to discover the correct entity ID. Do not assume entity IDs are sequential or reuse IDs from unrelated entities.", ge=1)],
) -> dict:
    """
    Get overview of all references to/from an entity (callers, callees, usage, etc.).

    Returns: total reference count, unique entities/files involved, and counts grouped by
    reference kind longname (e.g., 'C Call', 'C Callby', 'C Use', 'C Useby').

    The reference_kinds field uses longnames (language-qualified kind names) which can be
    used directly as filter strings in get_entity_references. For example, if the summary
    shows "C Call": 10, you can filter with refkindstring="C Call" to get exactly those
    references.

    Use this FIRST when exploring entity relationships to:
    - Discover what types of references exist (calls, uses, sets, etc.)
    - See reference counts before fetching details (avoids context window issues)
    - Identify which reference kinds are most common

    Workflow:
    1. get_entity_references_summary(ent_id) → see what's available
    2. get_entity_references(ent_id, refkindstring="C Callby") → get specific reference type
    3. Or: get_entity_references_by_file(ent_id) → see which files are involved
    """
    ent = require_db().ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")

    # Get all references
    all_refs = ent.refs()
    total_count = len(all_refs)

    # Group by reference kind (use longname for reference kinds to ensure
    # the returned strings work correctly with check() filtering)
    ref_kind_counts = {}
    unique_entities = set()
    unique_files = set()

    for ref in all_refs:
        ref_kind = ref.kind().longname()
        ref_kind_counts[ref_kind] = ref_kind_counts.get(ref_kind, 0) + 1

        # Track unique entities and files
        unique_entities.add(ref.ent().id())
        file_ent = ref.file()
        if file_ent:
            unique_files.add(file_ent.id())

    # Sort reference kinds by count (descending)
    sorted_kinds = sorted(ref_kind_counts.items(), key=lambda x: x[1], reverse=True)

    return {
        "total_references": total_count,
        "unique_entities_referenced": len(unique_entities),
        "unique_files": len(unique_files),
        "reference_kinds": {
            kind: count for kind, count in sorted_kinds
        },
    }

@mcp.tool(name="get_entity_references_by_file")
def get_entity_references_by_file(
    ent_id: Annotated[int, Field(description="The entity ID to get references grouped by file for. IMPORTANT: Entity IDs are arbitrary numbers assigned by Understand. Use lookup_entity_id(name='...') first to discover the correct entity ID. Do not assume entity IDs are sequential or reuse IDs from unrelated entities.", ge=1)],
    refkindstring: Annotated[Optional[str], Field(description="Optional reference kind filter string to filter references before grouping by file. Note: This filters based on how the entity references things, not how things reference the entity. For example, filtering by 'Type' on a class will show files where the class has Type references, not files that use the class as a type.")] = None,
    compact: Annotated[bool, Field(description="If True (default), returns only file_id and count. If False, includes file path.")] = True,
) -> dict:
    """
    Get references grouped by file location (shows which files use this entity).

    Returns: list of files with file_id, file path, and reference count per file.

    Use this when you need to:
    - See which files contain references to an entity
    - Find files with the most references (sorted by count)
    - Get file_id values for subsequent detailed queries

    Workflow:
    1. get_entity_references_summary(ent_id) → see overall reference statistics
    2. get_entity_references_by_file(ent_id) → see which files are involved
    3. get_entity_references(ent_id, file_id=...) → get references in specific file
    4. get_entity_source(file_id=...) → read the file if needed

    Files are sorted by reference count (most references first) to help prioritize exploration.

    Note on refkindstring: The filter applies to how the entity references other things (forward direction),
    not how other things reference the entity. If you get unexpected results (e.g., 0 files when filtering
    by a kind shown in get_entity_references_summary), try using get_entity_references with the same filter
    to understand what's being matched. For reverse lookups (who references this entity), use
    get_entity_references with refkindstring like 'useby', 'callby', etc.
    """
    ent = require_db().ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")

    # Get references with optional filtering
    all_refs = ent.refs(refkindstring=refkindstring)

    # Group by file
    file_refs = {}
    for ref in all_refs:
        file_ent = ref.file()
        if file_ent:
            file_id = file_ent.id()
            if file_id not in file_refs:
                file_refs[file_id] = {
                    "file_id": file_id,
                    "count": 0,
                }
                if not compact:
                    file_path = file_ent.relname() if file_ent.relname() else file_ent.longname()
                    file_refs[file_id]["file"] = file_path
            file_refs[file_id]["count"] += 1

    # Sort by count (descending)
    sorted_files = sorted(file_refs.values(), key=lambda x: x["count"], reverse=True)

    return {
        "total_files": len(sorted_files),
        "total_references": len(all_refs),
        "files": sorted_files,
    }

@mcp.tool(name="get_entity_references")
def get_entity_references(
    ent_id: Annotated[int, Field(description="The entity ID to get references for. IMPORTANT: Entity IDs are arbitrary numbers assigned by Understand. Use lookup_entity_id(name='...') first to discover the correct entity ID. Do not assume entity IDs are sequential or reuse IDs from unrelated entities.", ge=1)],
    refkindstring: Annotated[Optional[str], Field(description="Optional reference kind filter string. Filters by the TYPE OF RELATIONSHIP (e.g., 'C Call', 'C Use', 'C Define', 'C Set'). Use forward kinds (e.g., 'call', 'definein', 'use', 'set') for forward references, or reverse kinds (e.g., 'callby', 'useby', 'setby') for reverse references. Multiple kinds can be comma-separated. IMPORTANT: This filters by reference relationship type, NOT by entity kind. To filter by entity kind (e.g., Parameter, Function), use entkindstring instead.")] = None,
    entkindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string to filter by the KIND OF ENTITY being referenced (e.g., 'Function', 'Parameter', 'Variable', 'Class', 'Local Object'). This filters by what type of entity is referenced, not how it's referenced. Can be used alone without refkindstring. Example: entkindstring='Parameter' to list parameter entities of a function.")] = None,
    file_id: Annotated[Optional[Union[int, str]], Field(description="Optional file entity ID to filter references to only those occurring in a specific file.")] = None,
    unique: Annotated[bool, Field(description="If True, return only the first matching reference to each unique entity. Useful to avoid duplicates when the same entity is referenced multiple times. Can be used alone without other filters.")] = False,
    max_results: Annotated[int, Field(description="Maximum number of references to return. Default is 10 to keep responses concise.", ge=1, le=200)] = 10,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    Get detailed reference information for an entity with flexible filtering.

    Returns: list of references with referenced entity info, location (file_id, file path, line, column),
    and reference kind (longname). Includes total_count and truncated flag.

    Use this AFTER get_entity_references_summary to fetch specific reference types.
    The refkindstring filter uses token matching, so you can use:
    - Language-specific kinds from summary: refkindstring="C Call" (exact match from summary)
    - Generic kinds: refkindstring="call" (matches all call types across languages)
    - Forward references: refkindstring="call", "use", "set"
    - Reverse references: refkindstring="callby", "useby", "setby"
    - Multiple kinds: refkindstring="call,callby" (comma-separated)
    - Exclusions: refkindstring="call ~inactive" (exclude inactive/conditional calls)

    IMPORTANT: Understanding refkindstring vs entkindstring:
    - refkindstring: Filters by REFERENCE RELATIONSHIP TYPE (how entities are related)
      Examples: "Call", "Callby", "Use", "Useby", "Define", "Definein"
    - entkindstring: Filters by REFERENCED ENTITY KIND (what type of entity is referenced)
      Examples: "Function", "Parameter", "Variable", "Class", "Local Object"

    Common use cases:
    - List functions called by this function: refkindstring="call"
    - List callers of this function: refkindstring="callby"
    - List global objects set by this function: refkindstring="set", entkindstring="Global Object"

    Note: for finding children, prefer using list_entities instead.
    - list_entities(ent_id=<file_id>, kindstring="Function") → get functions in a specific file
    - list_entities(ent_id=<function_id>, kindstring="Parameter") → get parameters of a specific function

    Filtering options:
    - refkindstring: Filter by reference relationship type (use values from get_entity_references_summary)
    - entkindstring: Filter by referenced entity kind (e.g., "Parameter" to get function parameters)
    - file_id: Filter to specific file (use from get_entity_references_by_file)
    - unique: Get only first reference per unique entity (reduces duplicates)

    Each reference includes file_id so you can directly call get_entity_source without lookups.
    Supports pagination: use next_cursor from response to get more results.
    """
    # Coerce file_id to correct type (handle string inputs from LLMs)
    if file_id is not None:
        file_id = int(file_id)

    ent = require_db().ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")

    # Get references with optional filtering
    refs = ent.refs(refkindstring=refkindstring, entkindstring=entkindstring, unique=unique)

    # Filter by file_id if specified
    if file_id is not None:
        refs = [ref for ref in refs if ref.file() and ref.file().id() == file_id]

    # Apply pagination
    paginated_refs, total_count, truncated, next_cursor = paginate_results(refs, max_results, cursor)

    # Build concise reference summaries
    references = []
    for ref in paginated_refs:
        ref_ent = ref.ent()
        file_ent = ref.file()

        # Get file path (prefer relative path)
        file_path = None
        if file_ent:
            file_path = file_ent.relname() if file_ent.relname() else file_ent.longname()

        ref_info = {
            "referenced_entity": {
                "id": ref_ent.id(),
                "name": ref_ent.name(),
                "kind": ref_ent.kind().name(),
            },
            "location": {
                "file_id": file_ent.id() if file_ent else None,
                "file": file_path,
                "line": ref.line(),
                "column": ref.column(),
            },
            "reference_kind": ref.kind().longname(),
        }

        references.append(ref_info)

    response = {
        "references": references,
        "total_count": total_count,
        "truncated": truncated,
    }
    if next_cursor:
        response["next_cursor"] = next_cursor
    return response

@mcp.tool(name="list_metrics_summary")
def list_metrics_summary(
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string to list only metrics applicable to specific entity kinds (e.g., 'Function', 'File', 'Class').")] = None,
    include_disabled: Annotated[bool, Field(description="If True, returns all known metrics (enabled and disabled). If False (default), returns only enabled metrics. Note: metric values can be calculated even if metrics are disabled, but enabling controls visibility in the UI.")] = False,
    max_results: Annotated[int, Field(description="Maximum number of metrics to return. Default is 20 to keep responses concise. Use get_metric_details for specific metrics when descriptions are needed.", ge=1, le=500)] = 20,
    compact: Annotated[bool, Field(description="If True (default), returns only metric IDs. If False, includes metric names.")] = True,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    List available metrics without descriptions (efficient metric discovery).

    Returns: list of metric IDs and names only. No descriptions to preserve context window.

    Use this to discover available metrics before fetching values or descriptions:
    - Filter by entity kind (e.g., kindstring="Function") to see metrics for functions
    - Get metric IDs and short names to identify relevant metrics
    - Use returned IDs with get_metric_details for descriptions or get_entity_metrics for values

    Workflow:
    1. list_metrics_summary(kindstring="Function") → discover enabled metrics
    2. Review metric IDs/names to identify relevant ones
    3. get_metric_details(metric_ids=["Cyclomatic", "CountLine"]) → understand what they measure
    4. get_entity_metrics(ent_id, metric_ids=[...]) → get values for specific entity

    Note: Descriptions can be very long (65+ lines), so they're excluded from summaries.

    Metric Visibility: Some metrics may be disabled by default. Setting include_disabled=True shows
    all known metrics. Metric values can be calculated even if metrics are disabled (see get_entity_metrics),
    but enabling metrics controls their visibility in Understand's UI. If a metric you need isn't listed,
    try include_disabled=True or recommend the user enable it.
    """
    database = require_db()

    # Get list of available metrics (filter=True returns only enabled, filter=False returns all)
    if kindstring is None:
        all_metric_ids = understand.Metric.list(db=database, filter=not include_disabled)
    else:
        all_metric_ids = understand.Metric.list(kindstring, db=database, filter=not include_disabled)

    # Apply pagination
    paginated_ids, total_count, truncated, next_cursor = paginate_results(list(all_metric_ids), max_results, cursor)

    # Build summary (compact = IDs only, non-compact = IDs and names)
    if compact:
        metrics = paginated_ids
    else:
        metrics = []
        for metric_id in paginated_ids:
            try:
                metric_name = understand.Metric.name(metric_id)
                metrics.append({
                    "id": metric_id,
                    "name": metric_name,
                })
            except:
                # Skip metrics that can't be retrieved
                continue

    response = {
        "metrics": metrics,
        "total_count": total_count,
        "truncated": truncated,
    }
    if next_cursor:
        response["next_cursor"] = next_cursor
    return response

@mcp.tool(name="get_metric_details")
def get_metric_details(
    metric_ids: Annotated[List[str], Field(description="List of metric IDs to get full details for, including descriptions.")],
) -> List[dict]:
    """
    Get full metric descriptions (use selectively to avoid context window overflow).

    Returns: metric ID, name, and full description for each requested metric ID.

    Use this AFTER list_metrics_summary when you need to understand what specific metrics measure.
    Request details only for metrics you're actually interested in, as descriptions can be
    very long (e.g., 65+ lines shared across multiple metrics in plugin metric files).

    Common use cases:
    - Understanding what a metric measures before requesting values
    - Explaining metrics to users
    - Deciding which metrics are relevant for analysis

    Tip: Request descriptions for 5-10 metrics at a time max to avoid overwhelming the context.

    Note: If a metric ID is not found, it may be disabled. Metric values can still be calculated
    even if metrics are disabled (see get_entity_metrics), but you may want to recommend the user
    enable the metric for better visibility in Understand's UI.
    """
    results = []

    for metric_id in metric_ids:
        try:
            metric_name = understand.Metric.name(metric_id)
            metric_description = understand.Metric.description(metric_id)

            results.append({
                "id": metric_id,
                "name": metric_name,
                "description": metric_description,
            })
        except Exception as e:
            # Include error for invalid metric IDs
            results.append({
                "id": metric_id,
                "error": f"Metric not found: {str(e)}",
            })

    return results

@mcp.tool(name="get_entity_metrics")
def get_entity_metrics(
    ent_id: Annotated[int, Field(description="The entity ID to get metrics for. IMPORTANT: Entity IDs are arbitrary numbers assigned by Understand. Use lookup_entity_id(name='...') first to discover the correct entity ID. Do not assume entity IDs are sequential or reuse IDs from unrelated entities.", ge=1)],
    metric_ids: Annotated[List[str], Field(description="List of specific metric IDs to retrieve. Pass an empty list [] to return all available metrics for the entity. Note: metric values can be calculated even if the metric is disabled (not visible in UI).", default=[])] = [],
    compact: Annotated[bool, Field(description="If True (default), returns {metric_id: value}. If False, returns {metric_id: {name, value}}.")] = True,
) -> dict:
    """
    Get metric values for an entity (efficient metric retrieval).

    Returns: dictionary mapping metric IDs to values (with metric names for context).
    If metric_ids is provided, returns only those metrics. If None, returns all available.

    Use this to:
    - Get specific metric values after identifying relevant metrics via list_metrics_summary
    - Get all available metrics for an entity (useful for comprehensive analysis)
    - Analyze code quality, complexity, size, or other measured attributes

    Efficient workflow:
    1. list_metrics_summary(kindstring="Function") → discover available metrics
    2. Select relevant metric IDs (e.g., ["Cyclomatic", "CountLine", "MaxNesting"])
    3. get_entity_metrics(ent_id, metric_ids=[...]) → get values for specific metrics

    Or get all metrics at once: get_entity_metrics(ent_id) → returns everything available.
    Values include metric names so you don't need to cross-reference with metric details.

    Important: Metric values can be calculated even if metrics are disabled (not visible in UI).
    Enabling metrics controls visibility in Understand's UI, but doesn't affect whether values
    can be retrieved via the API. If a metric ID isn't found in list_metrics_summary, you can
    still try to get its value here if you know the metric ID.
    """
    ent = require_db().ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")

    results = {}
    if not metric_ids:
        # Get all available metrics for this entity (empty list = all metrics)
        available_metric_ids = ent.metrics()
        metric_values = ent.metric(available_metric_ids)

        # Build results
        for metric_id in available_metric_ids:
            value = metric_values.get(metric_id)
            if compact:
                results[metric_id] = value
            else:
                try:
                    metric_name = understand.Metric.name(metric_id)
                    results[metric_id] = {
                        "name": metric_name,
                        "value": value,
                    }
                except:
                    results[metric_id] = {
                        "value": value,
                    }
    else:
        # Get specific metrics - metric() returns dict for lists
        if len(metric_ids) == 1:
            # Single metric - returns value directly
            try:
                metric_value = ent.metric(metric_ids[0])
                if compact:
                    results[metric_ids[0]] = metric_value
                else:
                    metric_name = understand.Metric.name(metric_ids[0])
                    results[metric_ids[0]] = {
                        "name": metric_name,
                        "value": metric_value,
                    }
            except Exception as e:
                results[metric_ids[0]] = {"error": f"Failed to get metric: {str(e)}"}
        else:
            # Multiple metrics - returns dict
            metric_values = ent.metric(metric_ids)

            # Build results
            for metric_id in metric_ids:
                value = metric_values.get(metric_id)
                if compact:
                    results[metric_id] = value
                else:
                    try:
                        metric_name = understand.Metric.name(metric_id)
                        results[metric_id] = {
                            "name": metric_name,
                            "value": value,
                        }
                    except:
                        results[metric_id] = {
                            "value": value,
                        }

    return {
        "metrics": results,
        "count": len(results),
    }

@mcp.tool(name="get_project_overview")
def get_project_overview() -> dict:
    """
    Get high-level project statistics and overview information.

    Returns: project name, languages, file counts, entity counts by kind, and basic statistics.

    Use this as a starting point for project-level questions to understand:
    - What languages are in the project
    - How many files, functions, classes exist
    - Overall project size and structure

    This helps answer questions like "what is this project?" or provides context before
    diving into specific analysis (e.g., finding key functions, security risks).
    """
    database = require_db()

    # Get basic project info
    project_name = database.name()
    languages = list(database.language())

    # Count entities by kind
    files = database.files()
    all_entities = database.ents()

    # Count by common kinds (use filterable kind names to ensure
    # the returned strings work correctly with kindstring filters)
    kind_counts = {}
    for ent in all_entities:
        kind = get_filterable_kind_name(ent)
        kind_counts[kind] = kind_counts.get(kind, 0) + 1

    return {
        "name": project_name,
        "languages": languages,
        "total_files": len(files),
        "total_entities": len(all_entities),
        "entity_counts_by_kind": dict(sorted(kind_counts.items(), key=lambda x: x[1], reverse=True)[:20]),
    }

@mcp.tool(name="find_entities_by_metric")
def find_entities_by_metric(
    metric_id: Annotated[str, Field(description="The metric ID to filter by (e.g., 'Cyclomatic', 'CountLine', 'MaxNesting').")],
    arch_name: Annotated[Optional[str], Field(description="Optional architecture longname. If not provided, searches project-wide or within ent_id. If provided, filters to entities within that architecture (expanding containers to find nested entities). Use list_architectures_summary to discover available architectures. Can be combined with ent_id to find entities in a file that also belong to a specific architecture.")] = None,
    ent_id: Annotated[Optional[int], Field(description="Optional entity ID to search within (e.g., file, class, package). If provided, finds entities within this entity (expanding containers as needed). If kindstring is provided, filters to entities of that kind; if not provided, returns the entity itself and all its children. Can be combined with arch_name to filter results by architecture.")] = None,
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter (e.g., 'Function', 'File', 'Class').")] = None,
    min_value: Annotated[Optional[Union[float, int, str]], Field(description="Optional minimum metric value. Entities with metric values >= min_value will be returned.")] = None,
    max_value: Annotated[Optional[Union[float, int, str]], Field(description="Optional maximum metric value. Entities with metric values <= max_value will be returned.")] = None,
    order_by: Annotated[Literal["asc", "desc"], Field(description="Sort order: 'desc' for highest values first, 'asc' for lowest values first.")] = "desc",
    name_pattern: Annotated[Optional[str], Field(description="Optional name pattern to filter entities. Only entities whose names contain this substring (case-insensitive) are included. Example: 'parser' matches 'parseMarkdown', 'cmark_parser', 'ParserTest'.")] = None,
    max_results: Annotated[int, Field(description="Maximum number of results to return. Default is 10 to keep responses concise.", ge=1, le=200)] = 10,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    Find entities filtered and sorted by metric values. Can search project-wide, within a specific architecture, or within a specific entity.

    Returns: list of entities with their metric values, sorted by the metric.

    Use this for:
    - Project-wide queries: "What are the key functions in the project?" → metric_id="Cyclomatic", order_by="desc", arch_name=None
    - Architecture-scoped queries: "What are the key functions in src/?" → metric_id="Cyclomatic", order_by="desc", arch_name="Directory Structure/src" or "Directory Structure/<project>/src"
    - Entity-scoped queries: "What are the most complex functions in this file?" → metric_id="Cyclomatic", kindstring="Function", ent_id=<file_id>
    - Combined filters: "What are the most complex parser functions in src/?" → metric_id="Cyclomatic", kindstring="Function", name_pattern="parser", arch_name="Directory Structure/src"
    - Entity + architecture: "What are the most complex functions in this file that belong to Security Risk/High?" → metric_id="Cyclomatic", kindstring="Function", ent_id=<file_id>, arch_name="Security Risk/High"

    Note: Architecture paths vary by project structure. Some projects have flat layouts (Directory Structure/src),
    others have nested roots (Directory Structure/<project>/src). Use get_architecture_children to discover the actual hierarchy.

    If arch_name is provided, filters to entities within that architecture (expanding containers like files/classes to find nested entities like functions/methods).
    If ent_id is provided, filters to entities within that entity (expanding containers as needed). If kindstring is not provided, returns the entity itself and all its children.
    If both are provided, first expands entities within ent_id, then filters by architecture.
    If neither is provided, searches project-wide.

    Workflow:
    1. get_architecture_children("Directory Structure") → discover project structure (may show <project> child)
    2. list_metrics_summary(kindstring="Function") → discover relevant metrics
    3. find_entities_by_metric(metric_id="Cyclomatic", kindstring="Function", arch_name="Directory Structure/<project>/src") → find top entities in architecture
    4. find_entities_by_metric(metric_id="Cyclomatic", kindstring="Function", ent_id=<file_id>) → find top entities in a specific file
    5. get_entity_details(ent_id) → get details about specific entities
    """
    # Coerce parameters to correct types (handle string inputs from LLMs)
    if min_value is not None:
        min_value = float(min_value)
    if max_value is not None:
        max_value = float(max_value)

    database = require_db()

    # Get entities using common function
    entities, error_msg = get_entities_by_scope(database, ent_id, arch_name, kindstring)
    if error_msg:
        return {"error": error_msg}

    # Filter by name pattern early (before expensive metric operations)
    entities = filter_entities_by_name_pattern(entities, name_pattern)

    # Get metric values for all entities
    entity_metrics = []
    for ent in entities:
        try:
            value = ent.metric(metric_id)
            # Filter by min/max if specified
            if value is not None:
                if min_value is not None and value < min_value:
                    continue
                if max_value is not None and value > max_value:
                    continue

                entity_metrics.append({
                    "entity": {
                        "id": ent.id(),
                        "name": ent.name(),
                        "kind": ent.kind().name(),
                        "longname": ent.longname(),
                    },
                    "metric_value": value,
                })
        except:
            # Skip entities where metric isn't available
            continue

    # Sort by metric value
    reverse = (order_by == "desc")
    entity_metrics.sort(key=lambda x: x["metric_value"], reverse=reverse)

    # Apply pagination
    paginated, total_count, truncated, next_cursor = paginate_results(entity_metrics, max_results, cursor)

    response = {
        "entities": paginated,
        "total_found": total_count,
        "truncated": truncated,
    }
    if arch_name and total_count == 0:
        response["note"] = f"Architecture '{arch_name}' was resolved. No entities of kind '{kindstring}' (after expansion) were found matching the metric filter." if kindstring else f"Architecture '{arch_name}' has no entities matching the metric filter."
    if ent_id and total_count == 0:
        response["note"] = f"Entity {ent_id} was resolved. No entities of kind '{kindstring}' (after expansion) were found matching the metric filter." if kindstring else f"Entity {ent_id} has no entities matching the metric filter."
    if next_cursor:
        response["next_cursor"] = next_cursor
    return response

@mcp.tool(name="get_project_violations_summary")
def get_project_violations_summary() -> dict:
    """
    Get summary of all violations in the project grouped by check ID.

    Returns: total violation count, violations grouped by check_id with counts, and overall statistics.

    Use this FIRST to understand what violations exist before fetching detailed violation lists.
    This helps avoid fetching unnecessary large numbers of violations and identifies which check types are most common.

    Important: Violations are only available if:
    1. CodeCheck has been run with specific checks enabled (Checks → Open Codecheck in Understand)
    2. Violations have been imported from other tools via SARIF files

    If only UND_ERROR and UND_WARNING violations exist, or no violations at all, the user may need to:
    - Run CodeCheck with specific coding standards (MISRA, CERT, etc.) enabled
    - Import violations from other static analysis tools via SARIF format
    """
    database = require_db()

    # Get all violations
    all_violations = database.violations()
    total_count = len(all_violations)

    # Group by check_id
    check_id_counts = {}
    check_ids = set()
    for v in all_violations:
        check_id = v.check_id()
        check_ids.add(check_id)
        check_id_counts[check_id] = check_id_counts.get(check_id, 0) + 1

    # Sort by count (descending)
    sorted_checks = sorted(check_id_counts.items(), key=lambda x: x[1], reverse=True)

    return {
        "total_violations": total_count,
        "unique_check_ids": len(check_ids),
        "violations_by_check": {
            check_id: count for check_id, count in sorted_checks
        },
        "check_ids_list": [
            {"check_id": check_id, "count": count} for check_id, count in sorted_checks
        ],
    }

@mcp.tool(name="get_project_violations")
def get_project_violations(
    check_id: Annotated[Optional[str], Field(description="Optional violation check ID to filter by specific rule/check (e.g., 'MISRA_C_2012_8.4', 'UND_WARNING', 'CERT_C_2012_STR31_C'). Use get_project_violations_summary to see available check IDs.")] = None,
    file_path: Annotated[Optional[str], Field(description="Optional file path filter. Can be absolute path, relative path, or filename. Matches if the violation's file path contains this string. Note: violations store absolute file paths, and violations can exist outside of project files.")] = None,
    include_entity_info: Annotated[bool, Field(description="If True, resolves entity information for violations that have entity uniquenames. This adds minimal latency (~0.07ms per lookup, ~7ms for 100 violations) and is useful for violations like 'function too long' where you need the function entity ID. Default is False. Not all violations have entity information (some are for code regions rather than specific entities).")] = False,
    max_results: Annotated[int, Field(description="Maximum number of violations to return. Default is 20 to keep responses concise.", ge=1, le=500)] = 20,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    Get detailed violations for the project with location information.

    Returns: list of violations with check_id, file path, line/column, violation text, and optionally entity info.

    Use this AFTER get_project_violations_summary to fetch detailed violations for specific check types or files.
    This helps answer questions like:
    - "What are the security risks in this project?" → Filter violations for security-related checks
    - "What MISRA violations are there?" → Use check_id filter for MISRA rules
    - "Show me violations in a specific file" → Use file_path filter (can be filename, relative path, or absolute path)
    - "What violations are in src/main.c?" → Use file_path="src/main.c" or file_path="main.c"
    - "What functions are too long?" → Use include_entity_info=True to get function entity IDs

    Violations include: static analysis warnings, code standard violations (MISRA, CERT, AUTOSAR, etc.),
    security issues (MITRE CWE Top 25), and custom check violations.

    Note: Violations store absolute file paths as strings. The file_path parameter uses substring matching,
    so you can filter by filename, relative path, or absolute path. Violations can exist for files outside
    the project (e.g., from imported SARIF files).

    Entity information: Some violations (like "function too long") are associated with specific entities.
    Set include_entity_info=True to resolve entity IDs from uniquenames. This is expensive, so it defaults
    to False. Not all violations have entity information (some are for code regions rather than entities).
    When entity info is included, you can use get_entity_details or get_entity_source with the entity ID.
    """
    database = require_db()

    # Get all violations
    all_violations = database.violations()

    # Filter by check_id if specified
    if check_id:
        filtered_violations = [v for v in all_violations if check_id in v.check_id()]
    else:
        filtered_violations = all_violations

    # Filter by file_path if specified (substring match on absolute path)
    if file_path:
        filtered_violations = [
            v for v in filtered_violations
            if v.file() and file_path in v.file()
        ]

    # Apply pagination
    paginated_violations, total_count, truncated, next_cursor = paginate_results(filtered_violations, max_results, cursor)

    # Build violation summaries
    violations = []
    for viol in paginated_violations:
        violation_info = {
            "check_id": viol.check_id(),
            "file": viol.file(),
            "line": viol.line(),
            "column": viol.column(),
            "text": viol.text(),
        }

        # Optionally resolve entity information
        if include_entity_info:
            uniquename = viol.ent_uniquename()
            if uniquename:
                try:
                    ent = database.lookup_uniquename(uniquename)
                    if ent:
                        violation_info["entity"] = {
                            "id": ent.id(),
                            "name": ent.name(),
                            "kind": ent.kind().name(),
                        }
                except:
                    # Entity lookup failed, skip entity info
                    pass

        violations.append(violation_info)

    response = {
        "violations": violations,
        "total_count": total_count,
        "truncated": truncated,
    }
    if next_cursor:
        response["next_cursor"] = next_cursor
    return response

@mcp.tool(name="get_project_metrics")
def get_project_metrics(
    metric_ids: Annotated[List[str], Field(description="List of specific metric IDs to retrieve. Pass an empty list [] to return all available project-level metrics.")] = [],
) -> dict:
    """
    Get project-level metrics (metrics that apply to the entire project, not individual entities).

    Returns: dictionary of metric values with names for context.

    Use this to understand overall project characteristics:
    - Project size (total lines of code, files, functions)
    - Code quality metrics (average complexity, comment ratios)
    - Architecture metrics (propagation cost, core size)

    Workflow:
    1. list_metrics_summary() → discover available project metrics
    2. get_project_metrics(metric_ids=[...]) → get specific project-level metrics
    3. Use results to understand project characteristics before drilling into specific entities
    """
    database = require_db()

    # Handle None or empty list - both mean "get all metrics"
    if metric_ids is None:
        metric_ids = []

    if not metric_ids:
        # Get all available project-level metrics (empty list = all metrics)
        available_metric_ids = database.metrics()
        metric_values = database.metric(available_metric_ids)
    else:
        # Get specific metrics
        available_metric_ids = metric_ids
        if len(metric_ids) == 1:
            metric_values = {metric_ids[0]: database.metric(metric_ids[0])}
        else:
            metric_values = database.metric(metric_ids)

    # Build results with names
    results = {}
    for metric_id in available_metric_ids:
        try:
            metric_name = understand.Metric.name(metric_id)
            value = metric_values.get(metric_id) if isinstance(metric_values, dict) else metric_values
            results[metric_id] = {
                "name": metric_name,
                "value": value,
            }
        except:
            # Include value even if name can't be retrieved
            value = metric_values.get(metric_id) if isinstance(metric_values, dict) else metric_values
            results[metric_id] = {
                "value": value,
            }

    return {
        "metrics": results,
        "count": len(results),
    }

@mcp.tool(name="list_entities")
def list_entities(
    arch_name: Annotated[Optional[str], Field(description="Optional architecture longname. If not provided, searches project-wide or within ent_id. If provided, filters to entities within that architecture. Use list_architectures_summary to discover available architectures. Can be combined with ent_id to find entities in a file that also belong to a specific architecture.")] = None,
    ent_id: Annotated[Optional[int], Field(description="Optional entity ID to search within (e.g., file, class, package). If provided, finds entities within this entity (expanding containers as needed). If kindstring is provided, expands containers to find nested entities. Can be combined with arch_name to filter results by architecture.")] = None,
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter (e.g., 'Function', 'File', 'Class', 'Method'). If provided and arch_name or ent_id is provided, expands containers to find nested entities. If not provided and arch_name or ent_id is provided, returns direct members only (no expansion).")] = None,
    name_pattern: Annotated[Optional[str], Field(description="Optional name pattern to filter entities. Only entities whose names contain this substring (case-insensitive) are included. Example: 'parser' matches 'parseMarkdown', 'cmark_parser', 'ParserTest'.")] = None,
    max_results: Annotated[int, Field(description="Maximum number of entities to return. Default is 20 to keep responses concise.", ge=1, le=500)] = 20,
    compact: Annotated[bool, Field(description="If True (default), returns id, name, kind only. If False, includes longname.")] = True,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    List entities. Can search project-wide, within a specific architecture, within a specific entity, or combine entity and architecture filters.

    Returns: list of entities with id, name, kind, and basic info.

    Use this for:
    - Project-wide listing: "List all functions" → kindstring="Function"
    - Project-wide listing: "List all entities" → (no filters)
    - Architecture-scoped listing (direct members): "List files in src/" → arch_name="Directory Structure/src" or "Directory Structure/<project>/src" (no kindstring = direct members only)
    - Architecture-scoped listing (expanded): "List functions in src/" → kindstring="Function", arch_name="Directory Structure/src" or "Directory Structure/<project>/src" (kindstring = expands to find nested entities)
    - Entity-scoped listing (direct members): "List entities in this file" → ent_id=<file_id> (reports all nested entities)
    - Entity-scoped listing (expanded): "List parameters of this function" → ent_id=<function_id>, kindstring="Parameter" (reports only parameters)
    - Combined filters: "List parser functions in src/" → kindstring="Function", name_pattern="parser", arch_name="Directory Structure/src"
    - Entity + architecture: "List functions in this file that belong to Security Risk/High" → ent_id=<file_id>, kindstring="Function", arch_name="Security Risk/High"

    Note: Architecture paths vary by project structure. Some projects have flat layouts (Directory Structure/src),
    others have nested roots (Directory Structure/<project>/src). Use get_architecture_children to discover the actual hierarchy.

    Kindstring behavior:
    - When a kindstring is provided with an arch_name or ent_id, it expands containers (files/classes) to find nested entities matching the kindstring (functions/parameters)
    - When a kindstring is not provided with an arch_name or ent_id, it returns direct members only (no expansion)

    For finding entities by metric values (e.g., "most complex functions"), use find_entities_by_metric instead.

    Workflow:
    1. get_project_overview() → discover entity kinds that exist in the project
    2. list_entities(kindstring=<kindstring>) → get all entities of that kind (such as function, file, class, etc) in the project
    3. get_entity_details(ent_id) → get details about specific entities
    4. get_entity_source(ent_id) → read source code

    Filtering Examples:
    - list_entities(kindstring="Function", arch_name="Directory Structure/<project>/src") → get functions in src/ (expanded)
    - list_entities(arch_name="Directory Structure/<project>/src") → get files in src/ (direct members only)
    - list_entities(ent_id=<file_id>, kindstring="Function") → get functions in a specific file

    Returns minimal info (id, name, kind) to keep response concise. Use get_entity_details for more info.
    Supports pagination: use next_cursor from response to get more results.

    Filter examples: "function ~unresolved" (defined functions only), "file ~header" (source files only).
    """
    database = require_db()

    # Get entities using common function
    entities, error_msg = get_entities_by_scope(database, ent_id, arch_name, kindstring)
    if error_msg:
        return {"error": error_msg}

    # Filter by name pattern early
    entities = filter_entities_by_name_pattern(entities, name_pattern)

    # Apply pagination
    paginated_entities, total_count, truncated, next_cursor = paginate_results(entities, max_results, cursor)

    # Build entity summaries
    results = []
    for ent in paginated_entities:
        entry = {
            "id": ent.id(),
            "name": ent.name(),
            "kind": ent.kind().name(),
        }
        if not compact:
            entry["longname"] = ent.longname()
        results.append(entry)

    response = {
        "entities": results,
        "total_count": total_count,
        "truncated": truncated,
    }
    if arch_name and total_count == 0:
        response["note"] = f"Architecture '{arch_name}' was resolved. No entities of kind '{kindstring}' (after expansion) were found." if kindstring else f"Architecture '{arch_name}' has no direct members."
    if ent_id and total_count == 0:
        response["note"] = f"Entity {ent_id} was resolved. No entities of kind '{kindstring}' (after expansion) were found." if kindstring else f"Entity {ent_id} has no children."
    if next_cursor:
        response["next_cursor"] = next_cursor
    return response

@mcp.tool(name="list_architectures_summary")
def list_architectures_summary() -> dict:
    """
    List root architectures in the project. An architecture is a hierarchical grouping of entities.

    Returns: list of root architecture names. Most projects have only 1-3 root architectures.

    All projects with at least one file will have a "Directory Structure" architecture showing
    the folder structure. Additional architectures may be created manually or via plugins.

    Common architectures:
    - "Directory Structure" - built-in folder hierarchy (always present)
    - Custom architectures - created by users or plugins (e.g., functional decomposition, git metadata)

    Workflow:
    1. list_architectures_summary() → discover available architectures
    2. get_architecture_details(arch_name) → get hierarchy and entity information
    3. list_entities(arch_name=arch_name) → explore files/entities in an architecture (direct members only)
    """
    database = require_db()

    root_archs = database.root_archs()
    architecture_names = [arch.name() for arch in root_archs]

    return {
        "architectures": architecture_names,
        "count": len(architecture_names),
    }

@mcp.tool(name="directory_to_architecture")
def directory_to_architecture(
    directory_path: Annotated[str, Field(description="Full path to a directory to convert to an architecture longname.")],
) -> dict:
    """
    Convert a directory path to its corresponding architecture longname in the Directory Structure.

    Returns: the architecture longname if the directory exists in the project, or an error if not found.

    Use this to quickly look up the architecture for a specific folder path without navigating
    the architecture tree manually.

    Example:
    - directory_to_architecture("/project/src/utils") → {"arch_longname": "Directory Structure/src/utils"}

    After getting the longname, use get_architecture_details or list_entities to explore.
    """
    database = require_db()

    # Convert to relative path within the project
    relative_path = database.relative_file_name(directory_path)
    if not relative_path:
        return {"error": f"Directory '{directory_path}' is not within the project."}

    # Construct the architecture longname
    arch_longname = f"Directory Structure/{relative_path}"

    # Verify the architecture exists
    arch = database.lookup_arch(arch_longname)
    if arch is None:
        return {"error": f"No architecture found for directory '{directory_path}'. The directory may not exist or may not contain any analyzed files."}

    return {"arch_longname": arch_longname}

# Key metrics always included in get_architecture_details (compact and non-compact).
# CountArchChildren/CountArchEnts = direct; CountArchChildrenRecursive/CountArchEntsRecursive = subtree (from arch_metrics.upy).
KEY_METRICS_ALWAYS = [
    "CountArchEnts",
    "CountArchEntsRecursive",
    "CountArchChildren",
    "CountArchChildrenRecursive",
    "CountLineCode",
    "PercentOfRootCodeLines",
    "CountDepRefsIn",
    "CountDepRefsOut",
]

# Additional key metrics when compact=False (size, structure, complexity, percentages).
KEY_METRICS_WHEN_NOT_COMPACT = [
    "CountLine",
    "CountDeclFile",
    "CountDeclFunction",
    "CountDeclClass",
    "CountDeclSubprogram",  # Ada, Basic, Fortran, Jovial, Pascal (instead of CountDeclFunction)
    "SumCyclomatic",
    "PercentOfRootEntities",
    "PercentOfParentCodeLines",
    "RatioCommentToCode",
    "CountDepsIn",
    "CountDepsOut",
    "ArchMaxChildDepth",  # max depth of subtree (shape: deep vs shallow; CountArchChildrenRecursive is in KEY_METRICS_ALWAYS)
]

# When compact=True, limit entity_percentages_by_kind to this many kinds (by percentage descending).
ENTITY_KIND_TOP_N_COMPACT = 10

# Max descendant architectures to list in get_architecture_details (BFS, may include grandchildren).
CHILDREN_MAX_COMPACT = 10
CHILDREN_MAX_NOT_COMPACT = 25

# Max sorted_entities (direct entities, ranked by _key_entities_ranked_header logic) to list.
SORTED_ENTITIES_MAX_COMPACT = 5
SORTED_ENTITIES_MAX_NOT_COMPACT = 15


@mcp.tool(name="get_architecture_details")
def get_architecture_details(
    arch_name: Annotated[str, Field(description="Longname of the architecture to get details for. Use list_architectures_summary to discover root architecture names.")],
    compact: Annotated[bool, Field(description="If True (default), returns shorter lists.")] = True,
) -> dict:
    """
    Get details for an architecture (structure and contents).

    Returns:
    - key_metrics: such as direct and recursive counts of entities and child architectures.
      Most architecture metrics are subtree aggregates.
    - descendant_architectures: a BFS list of descendant architectures ordered by percent_code_lines.
    - sorted_entities: first N (N depends on compact) of the architecture's direct entities (total
      count is key_metrics.CountArchEnts), ordered by incoming dependencies (header preferred).
    - entity_percentages_by_kind: percent of direct entities by kind (omitted if none)
    - is_root: whether this is a root architecture

    Each of the list fields will be omitted if empty, and each uses the compact parameter to determine
    maximum results shown.

    Note: most architecture's direct entities are files. To find nested entities like functions within
    a folder, use list_entities with kindstring and arch_name, or find_entities_by_metric with arch_name.

    Workflow:
    1. list_architectures_summary() → discover root architectures
    2. get_architecture_details(arch_name) → determine architecture structure and contents

    Follow-up for descendant_architectures (child_longname from each item's longname):
    - get_architecture_details(arch_name=child_longname) → details for a shown child
    - get_architecture_children(arch_name=arch_name) → more direct children than shown
    - get_architecture_intersection(source_arch_name=arch_name, group_by_arch_name=arch_name) → more descendants than shown

    Follow-up for sorted_entities (entity_id from each item):
    - get_entity_details(ent_id=entity_id) → details for a shown entity
    - get_entity_source(ent_id=entity_id) → source for a shown entity
    - list_entities(arch_name=arch_name) → more direct entities than shown
    - find_entities_by_metric(metric_id=..., arch_name=arch_name) → more entities by metric (e.g. complexity)
    """
    database = require_db()

    arch = database.lookup_arch(arch_name)
    if arch is None:
        return {"error": architecture_not_found_error(database, arch_name)}

    entities = arch.ents(False)

    # Percent of direct entities by kind (omit if no direct entities)
    entity_percentages_by_kind = {}
    entity_percentages_by_kind_note = ""
    if entities:
        counts_by_kind = {}
        for ent in entities:
            kind_name = get_filterable_kind_name(ent)
            counts_by_kind[kind_name] = counts_by_kind.get(kind_name, 0) + 1
        total = len(entities)
        sorted_pct = sorted(counts_by_kind.items(), key=lambda x: -x[1])
        num_kinds = len(sorted_pct)
        if compact:
            sorted_pct = sorted_pct[:ENTITY_KIND_TOP_N_COMPACT]
            if num_kinds > ENTITY_KIND_TOP_N_COMPACT:
                entity_percentages_by_kind_note = f"Truncated: showing top {ENTITY_KIND_TOP_N_COMPACT} of {num_kinds} entity kinds by percentage."
        entity_percentages_by_kind = {k: round(100.0 * c / total, 2) for k, c in sorted_pct}

    # Key metrics: always-on list, plus optional list when not compact
    metric_ids = list(KEY_METRICS_ALWAYS)
    if not compact:
        metric_ids = metric_ids + list(KEY_METRICS_WHEN_NOT_COMPACT)

    key_metrics = {}
    try:
        if metric_ids:
            metric_values = arch.metric(metric_ids)
            if not isinstance(metric_values, dict):
                metric_values = {metric_ids[0]: metric_values}
            for mid in metric_ids:
                value = metric_values.get(mid) if isinstance(metric_values, dict) else None
                if value is not None:
                    key_metrics[mid] = value
    except Exception:
        pass

    # Relevance-ordered list of descendant archs (not necessarily direct children), sorted by percent of this arch's code lines, trimmed to max_results
    descendant_list = []
    descendant_architectures_note = ""
    try:
        arch_loc = arch.metric("CountLineCode")
        arch_loc = int(arch_loc) if arch_loc is not None else 0
        max_depth = arch.metric("ArchMaxChildDepth") or 0
        max_descendants = CHILDREN_MAX_COMPACT if compact else CHILDREN_MAX_NOT_COMPACT
        descendant_archs, depth = _get_architecture_children_bfs(arch, max_descendants)
        if descendant_archs and arch_loc > 0:
            parts = []
            if max_depth > 0:
                parts.append(f"Expanded to depth {depth} of {max_depth}")
            if len(descendant_archs) > max_descendants:
                parts.append(f"Truncated: showing {max_descendants} of {len(descendant_archs)} descendant architectures")
            if parts:
                descendant_architectures_note = ". ".join(parts)
            pairs = []
            for child in descendant_archs:
                c_loc = child.metric("CountLineCode")
                c_loc = int(c_loc) if c_loc is not None else 0
                pct = round(100.0 * c_loc / arch_loc, 2)
                pairs.append((child.longname(), pct))
            pairs.sort(key=lambda x: -x[1])
            pairs = pairs[:max_descendants]
            descendant_list = [{"longname": ln, "percent_code_lines": pct} for ln, pct in pairs]
    except Exception:
        pass

    result = {
        "is_root": arch.parent() is None,
        "key_metrics": key_metrics,
    }
    if entity_percentages_by_kind:
        result["entity_percentages_by_kind"] = entity_percentages_by_kind
        if entity_percentages_by_kind_note:
            result["entity_percentages_by_kind_note"] = entity_percentages_by_kind_note
    if descendant_list:
        result["descendant_architectures"] = descendant_list
        if descendant_architectures_note:
            result["descendant_architectures_note"] = descendant_architectures_note

    # Sorted entities: direct entities, ranked by _key_entities_ranked_header (header preferred, stem match, incoming deps, size)
    sorted_entities_list = []
    try:
        ranked = _key_entities_ranked_header(arch, recursive=False)
        max_sorted = SORTED_ENTITIES_MAX_COMPACT if compact else SORTED_ENTITIES_MAX_NOT_COMPACT
        for ent in ranked[:max_sorted]:
            sorted_entities_list.append({"entity_id": ent.id(), "name": ent.name() or ""})
    except Exception:
        pass
    if sorted_entities_list:
        result["sorted_entities"] = sorted_entities_list

    return result

@mcp.tool(name="get_architectures_for_entity")
def get_architectures_for_entity(
    ent_id: Annotated[int, Field(description="The entity ID to find architectures for. IMPORTANT: Entity IDs are arbitrary numbers assigned by Understand. Use lookup_entity_id(name='...') first to discover the correct entity ID. Do not assume entity IDs are sequential or reuse IDs from unrelated entities.", ge=1)],
    implicit: Annotated[bool, Field(description="If True, also returns architectures that contain any of the entity's parents. Default is False.",)] = False,
) -> dict:
    """
    Find all architectures containing an entity (reverse lookup for metadata/tagging).

    Returns: list of architectures that contain this entity.

    Use this for reverse lookup to discover metadata about an entity:
    - Find modification date: look for Calendar architecture containing the file
    - Find git author: look for Git Author architecture containing the file
    - Find functional group: find which functional decomposition architecture contains the file
    - Understand folder structure: see which Directory Structure nodes contain the file

    This is a key tool for using architectures as a tagging/metadata system. For example:
    - Calendar architecture groups files by modification date → find date by finding which Calendar node contains the file
    - Git Author architecture groups files by author → find author by finding which Git Author node contains the file

    Workflow:
    1. lookup_entity_id(name="main.c") → find file entity
    2. get_architectures_for_entity(ent_id) → find all architectures (including metadata architectures)
    3. Filter results to find relevant architecture (e.g., Calendar for date, Git Author for author)
    """
    database = require_db()

    ent = database.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")

    archs = database.archs(ent, implicit)

    architectures = []
    for arch in archs:
        architectures.append({
            "longname": arch.longname(),
        })

    return {
        "architectures": architectures,
        "count": len(architectures),
    }

@mcp.tool(name="get_architecture_children")
def get_architecture_children(
    arch_name: Annotated[str, Field(description="Longname of the architecture to get children for. Use list_architectures_summary to discover root architecture names.")],
    max_results: Annotated[int, Field(description="Maximum number of child architectures to return. Default is 30.", ge=1, le=500)] = 30,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    Get child architectures of a parent architecture (navigate hierarchy).

    Returns: list of child architecture names. To get details on a child, construct its
    longname as "{parent_longname}/{child_name}" and pass it to get_architecture_details.

    Use this to navigate the hierarchical structure of architectures:
    - Navigate Directory Structure folders (e.g., "Directory Structure" → children like "src", "tests")
    - Explore functional decomposition hierarchies
    - Browse Calendar or Git Author architecture hierarchies

    Workflow:
    1. get_architecture_details(arch_name="Directory Structure") → see it has children
    2. get_architecture_children(arch_name="Directory Structure") → get child names like ["src", "tests"]
    3. get_architecture_details(arch_name="Directory Structure/src") → details for "src" child
    4. list_entities(arch_name="Directory Structure/src") → files in that folder (direct members only)

    Supports pagination: use next_cursor from response to get more results.
    """
    database = require_db()

    arch = database.lookup_arch(arch_name)
    if arch is None:
        return {"error": architecture_not_found_error(database, arch_name)}

    children = arch.children()

    # Apply pagination
    paginated_children, total_count, truncated, next_cursor = paginate_results(children, max_results, cursor)
    child_names = [child.name() for child in paginated_children]

    response = {
        "children": child_names,
        "count": len(child_names),
        "total_count": total_count,
        "truncated": truncated,
    }
    if next_cursor:
        response["next_cursor"] = next_cursor
    return response

@mcp.tool(name="get_architecture_metrics")
def get_architecture_metrics(
    arch_name: Annotated[str, Field(description="Longname of the architecture to get metrics for. Use list_architectures_summary to discover root architecture names.")],
    metric_ids: Annotated[List[str], Field(description="List of specific metric IDs to retrieve. Pass an empty list [] to return all available metrics for the architecture.", default=[])] = [],
) -> dict:
    """
    Get metric values for an architecture (aggregate metrics for all entities in the architecture).

    Returns: dictionary mapping metric IDs to values (with metric names for context).

    Discovering architecture metrics:
    - Use list_metrics_summary() with no kindstring to see all metrics (built-in and plugin).
    - Built-in metrics: Any metric that applies to the project also applies to architectures.
    - Common architecture metrics include: CountLine, CountLineCode, CountStmt, SumCyclomatic,
      MaxCyclomatic, AvgCyclomatic, CountFile, CountClass, CountFunction, RatioCommentToCode.
    - Architecture-specific plugin metrics:
      - CountArchEnts, CountArchEntsRecursive - entity counts
      - ArchCurDepth, ArchMaxChildDepth, CountArchChildren - hierarchy info
      - GitCohesion - git cohesion metric (architecture-only, requires git)
    - Git metrics (if git is configured, also valid for files):
      - GitCommits, GitAuthors, GitOwnership, GitMajorContributors, GitMinorContributors
      - GitDaysSinceCreated, GitDaysSinceLastModified

    Common use cases:
    - "What's the total lines of code in the src folder?" → Get CountLine for Directory Structure/src
    - "What's the complexity of module X?" → Get SumCyclomatic or AvgCyclomatic
    - "Which folder has the most files?" → Compare CountFile across architectures

    Note: Metrics that don't apply to an architecture return null (e.g., entity-specific metrics
    like Cyclomatic for a folder). Some metrics may need to be enabled in Project Configuration/Metrics.
    """
    database = require_db()

    arch = database.lookup_arch(arch_name)
    if arch is None:
        return {"error": architecture_not_found_error(database, arch_name)}

    results = {}

    if not metric_ids:
        # Get all available metrics (empty list = all metrics)
        available_metric_ids = arch.metrics()
        metric_values = arch.metric(available_metric_ids)

        for metric_id in available_metric_ids:
            try:
                metric_name = understand.Metric.name(metric_id)
                value = metric_values.get(metric_id)
                results[metric_id] = {
                    "name": metric_name,
                    "value": value,
                }
            except:
                value = metric_values.get(metric_id)
                results[metric_id] = {
                    "value": value,
                }
    else:
        # Get specific metrics
        if len(metric_ids) == 1:
            try:
                metric_value = arch.metric(metric_ids[0])
                metric_name = understand.Metric.name(metric_ids[0])
                results[metric_ids[0]] = {
                    "name": metric_name,
                    "value": metric_value,
                }
            except Exception as e:
                results[metric_ids[0]] = {
                    "error": f"Failed to get metric: {str(e)}",
                }
        else:
            metric_values = arch.metric(metric_ids)

            for metric_id in metric_ids:
                try:
                    metric_name = understand.Metric.name(metric_id)
                    value = metric_values.get(metric_id)
                    results[metric_id] = {
                        "name": metric_name,
                        "value": value,
                    }
                except:
                    value = metric_values.get(metric_id)
                    results[metric_id] = {
                        "value": value,
                    }

    return {
        "metrics": results,
        "count": len(results),
        "architecture": arch.longname(),
    }

@mcp.tool(name="get_architecture_intersection")
def get_architecture_intersection(
    source_arch_name: Annotated[str, Field(description="Longname of the source architecture to get entities from (e.g., 'Git Owner/Jason Haslam', 'Key Functions').")],
    group_by_arch_name: Annotated[str, Field(description="Longname of the architecture to group entities by (e.g., 'Directory Structure', 'Git Owner'). Defaults to 'Directory Structure' if not specified.", default="Directory Structure")] = "Directory Structure",
    recursive: Annotated[bool, Field(description="If True, includes entities from child architectures of source_arch_name and groups by all children (recursive) of group_by_arch_name. If False, only direct entities and direct children. Default is True.", default=True)] = True,
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter (e.g., 'File', 'Function', 'Class') to filter entities before grouping. If provided, automatically expands containers in source_arch to find nested entities matching the kindstring.")] = None,
    max_results: Annotated[int, Field(description="Maximum number of groups to return. Groups are sorted by count (descending). Default is 50.", ge=1, le=500, default=50)] = 50,
    compact: Annotated[bool, Field(description="If True (default), returns only group_name, count, and percentage. If False, includes sample_entities for each group.")] = True,
) -> dict:
    """
    Get entity counts grouped by intersecting two architectures.

    Finds entities in source_arch_name and groups them by which child of group_by_arch_name
    they belong to. Useful for path-based analysis, contributor analysis, and cross-architecture
    queries (e.g., "What directories does this contributor work in?").

    Returns: groups with counts and percentages, sorted by count (descending), plus total_entities.

    Expansion behavior: If kindstring is provided, automatically expands containers in source_arch
    to find nested entities (e.g., Functions inside Files). Destination architecture always uses
    implicit membership to find entities through parent containers.

    Example: {"total_entities": 200, "groups": [{"group_name": "Directory Structure/src", "count": 150, "percentage": 75.0}]}
    """
    database = require_db()

    # Look up source architecture
    source_arch = database.lookup_arch(source_arch_name)
    if source_arch is None:
        return {"error": architecture_not_found_error(database, source_arch_name)}

    # Look up group-by architecture
    group_by_arch = database.lookup_arch(group_by_arch_name)
    if group_by_arch is None:
        return {"error": architecture_not_found_error(database, group_by_arch_name)}

    # Get entities from source architecture
    if kindstring:
        # Use expansion function to find entities matching kindstring
        entities = expand_containers_in_architecture(source_arch, kindstring, recursive)
    else:
        # Get all entities directly (no kindstring filter)
        entities = set(source_arch.ents(recursive))

    # Count entities per group and optionally collect sample entities
    group_counts = {}
    group_entities = {} if not compact else None  # Only track entities if not compact
    total_processed = 0

    # For each entity, find which group architectures it belongs to
    for ent in entities:
        total_processed += 1

        # Get all architectures this entity belongs to (with implicit=True for container expansion)
        entity_archs = database.archs(ent, implicit=True)

        # Find matching group architectures - those that start with group_by_arch_name
        matching_groups = []
        for arch in entity_archs:
            arch_longname = arch.longname()

            # Check if this architecture matches group_by_arch_name
            if arch_longname == group_by_arch_name:
                # Exact match
                matching_groups.append(arch_longname)
            elif arch_longname.startswith(group_by_arch_name + "/"):
                # This is a child architecture of group_by_arch
                if recursive:
                    # Include all descendants
                    matching_groups.append(arch_longname)
                else:
                    # Only include direct children (one level deep)
                    # Check if there's only one "/" after group_by_arch_name
                    suffix = arch_longname[len(group_by_arch_name) + 1:]
                    if "/" not in suffix:
                        # Direct child (no further nesting)
                        matching_groups.append(arch_longname)

        # Count in all matching groups and optionally collect sample entities
        for group_name in matching_groups:
            group_counts[group_name] = group_counts.get(group_name, 0) + 1
            if not compact:
                if group_name not in group_entities:
                    group_entities[group_name] = []
                if len(group_entities[group_name]) < 5:  # Limit sample size
                    group_entities[group_name].append({
                        "id": ent.id(),
                        "name": ent.name(),
                        "kind": ent.kind().name(),
                    })

    # Build results sorted by count
    groups = []
    for group_name, count in sorted(group_counts.items(), key=lambda x: -x[1]):
        if count > 0:  # Only include groups with entities
            percentage = (count / total_processed * 100) if total_processed > 0 else 0.0
            group_data = {
                "group_name": group_name,
                "count": count,
                "percentage": round(percentage, 2),
            }
            if not compact and group_name in group_entities:
                group_data["sample_entities"] = group_entities[group_name]
            groups.append(group_data)

    # Limit results
    if len(groups) > max_results:
        groups = groups[:max_results]

    return {
        "total_entities": total_processed,
        "groups": groups,
    }

@mcp.tool(name="get_entities_grouped_by_pattern")
def get_entities_grouped_by_pattern(
    pattern: Annotated[str, Field(description="Regular expression pattern to match entity names. Can include capture groups for grouping. Use (?i) prefix for case-insensitive matching.")],
    arch_name: Annotated[Optional[str], Field(description="Optional architecture longname. If not provided, searches project-wide. If provided, filters to entities within that architecture. Use list_architectures_summary to discover available architectures.")] = None,
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter (e.g., 'File', 'Function', 'Class').")] = None,
    group_by: Annotated[Literal["match", "group"], Field(description="How to group: 'match' groups by full match, 'group' uses capture groups. Default is 'match'.", default="match")] = "match",
    group_index: Annotated[Optional[int], Field(description="If group_by='group', which capture group to use (1-indexed). If not specified, uses all groups as tuple key.", ge=1)] = None,
    recursive: Annotated[bool, Field(description="If True and arch_name is provided, includes entities from child architectures. Default is True.", default=True)] = True,
    max_results: Annotated[int, Field(description="Maximum number of groups to return. Groups are sorted by count (descending). Default is 50.", ge=1, le=500, default=50)] = 50,
    compact: Annotated[bool, Field(description="If True (default), returns only group, count, and percentage. If False, includes sample_entities for each group.")] = True,
) -> dict:
    """
    Group entities by regex pattern matches on their names (filenames for files). Can search project-wide or within a specific architecture.

    Similar to get_architecture_intersection, but groups by filename patterns instead of
    architecture membership. Use this for filename-based grouping (extensions, naming conventions).
    Use get_architecture_intersection for path-based grouping by Directory Structure.

    Returns: groups with counts, percentages, and sample entities, sorted by count (descending).

    Use this for:
    - Project-wide grouping: "Group all files by extension" → pattern=r"\\.(\\w+)$", group_by="group", group_index=1, arch_name=None
    - Architecture-scoped grouping: "Group files by extension in src/" → pattern=r"\\.(\\w+)$", group_by="group", group_index=1, arch_name="Directory Structure/src"

    IMPORTANT: This tool matches against ent.name() which for files is just the filename
    (e.g., "file.cpp"), NOT the file path. Patterns like ^src/ or ^dep/ will NOT work
    because they're path patterns. For path-based grouping, use get_architecture_intersection
    with Directory Structure architecture.

    Examples:
    - File extension: pattern=r"\\.(\\w+)$", group_by="group", group_index=1
    - Test files: pattern=r".*_test\\.(\\w+)$", group_by="group", group_index=1
    - Build files: pattern="(?i)(CMakeLists\\.txt|Makefile|package\\.json)", group_by="match"
    - Documentation: pattern="(?i)(readme|changelog|license).*", group_by="match"

    For path-based patterns (e.g., "files in src/ directory"), use:
    get_architecture_intersection(source_arch, "Directory Structure", group_by_arch_name="Directory Structure")
    """
    database = require_db()

    # Get entities (project-wide or architecture-scoped)
    if arch_name:
        # Architecture-scoped
        arch = database.lookup_arch(arch_name)
        if arch is None:
            return {"error": architecture_not_found_error(database, arch_name)}

        if kindstring:
            entities = expand_containers_in_architecture(arch, kindstring, recursive)
        else:
            entities = set(arch.ents(recursive))
    else:
        # Project-wide
        if kindstring:
            entities = set(database.ents(kindstring))
        else:
            entities = set(database.ents())

    # Compile regex
    try:
        regex = re.compile(pattern)
    except re.error as e:
        return {"error": f"Invalid regex pattern: {e}"}

    # Group entities by pattern match
    groups = {}
    total_matched = 0

    for ent in entities:
        name = ent.name()
        match = regex.search(name)

        if match:
            total_matched += 1

            if group_by == "match":
                # Group by full match
                group_key = match.group(0)
            else:
                # Group by capture group
                if group_index:
                    if group_index <= len(match.groups()):
                        group_key = match.group(group_index)
                    else:
                        continue  # Invalid group index
                else:
                    # Use all groups as tuple key
                    group_key = match.groups()

            # Convert tuple to string for JSON serialization
            if isinstance(group_key, tuple):
                group_key = "|".join(str(g) for g in group_key)

            if group_key not in groups:
                groups[group_key] = {"count": 0}
                if not compact:
                    groups[group_key]["entities"] = []  # Store sample entities (limit to avoid huge responses)

            groups[group_key]["count"] += 1
            if not compact and len(groups[group_key]["entities"]) < 10:  # Limit sample size
                groups[group_key]["entities"].append({
                    "id": ent.id(),
                    "name": ent.name(),
                    "kind": ent.kind().name(),
                    "longname": ent.longname(),
                })

    # Build results sorted by count
    result_groups = []
    for group_key, data in sorted(groups.items(), key=lambda x: -x[1]["count"]):
        if len(result_groups) >= max_results:
            break

        percentage = (data["count"] / total_matched * 100) if total_matched > 0 else 0
        group_data = {
            "group": str(group_key),
            "count": data["count"],
            "percentage": round(percentage, 2),
        }
        if not compact:
            group_data["sample_entities"] = data["entities"][:5]  # Limit to 5 samples
        result_groups.append(group_data)

    return {
        "total_matched": total_matched,
        "total_entities": len(entities),
        "groups": result_groups,
    }

def tokenize_name(name: str, include_numbers: bool = True, numbers_as_separators: bool = False) -> list[str]:
    """
    Tokenize an entity name into words, handling multiple naming conventions.

    Handles:
    - camelCase: "parseMarkdown" → ["parse", "markdown"]
    - snake_case: "cmark_parser" → ["cmark", "parser"]
    - kebab-case: "test-utils" → ["test", "utils"]
    - PascalCase: "MyClass" → ["my", "class"]
    - Spaces: "test utils" → ["test", "utils"]
    - Mixed: "XMLParser_test" → ["xml", "parser", "test"]
    - Numbers: Configurable handling (see parameters)

    Args:
        name: Entity name to tokenize
        include_numbers: If True, includes numbers as separate tokens or part of words.
                        If False, removes numbers entirely.
        numbers_as_separators: If True, treats numbers as word separators (e.g., "test2util" → ["test", "util"]).
                              If False, keeps numbers with adjacent letters (e.g., "test2util" → ["test2", "util"]).
                              Only applies if include_numbers=True.

    Returns:
        List of lowercase word tokens

    Number handling examples:
    - include_numbers=True, numbers_as_separators=False:
      "test2util" → ["test2", "util"]
      "v1_parser" → ["v1", "parser"]
      "file123" → ["file123"]

    - include_numbers=True, numbers_as_separators=True:
      "test2util" → ["test", "2", "util"]
      "v1_parser" → ["v", "1", "parser"]
      "file123" → ["file", "123"]

    - include_numbers=False:
      "test2util" → ["test", "util"]
      "v1_parser" → ["v", "parser"]
      "file123" → ["file"]
    """
    # Remove file extensions
    name = re.sub(r'\.[^.]+$', '', name)

    # Handle numbers based on configuration
    if not include_numbers:
        # Remove all numbers
        name = re.sub(r'\d+', '', name)
    elif numbers_as_separators:
        # Insert separators around numbers to split words
        # "test2util" → "test_2_util"
        name = re.sub(r'(\d+)', r'_\1_', name)

    # Split on underscores, hyphens, spaces, and numbers (if configured as separators)
    if numbers_as_separators:
        parts = re.split(r'[_\-\s]+', name)
        # Further split on number boundaries that were inserted
        all_parts = []
        for part in parts:
            if not part:
                continue
            # Split on inserted number separators
            sub_parts = re.split(r'_+', part)
            for sub_part in sub_parts:
                if sub_part:
                    all_parts.append(sub_part)
        parts = all_parts
    else:
        parts = re.split(r'[_\-\s]+', name)

    # Further split camelCase/PascalCase and extract words
    words = []
    for part in parts:
        if not part:
            continue

        # Split on camelCase boundaries (lowercase to uppercase, or uppercase to lowercase)
        # Handle acronyms (multiple uppercase letters)
        # Pattern explanation:
        # - [A-Z]?[a-z]+ : Matches lowercase words, optionally starting with uppercase (e.g., "parse", "Markdown")
        # - [A-Z]+(?=[A-Z]|$) : Matches acronyms (multiple uppercase) followed by another uppercase or end (e.g., "XML", "HTML")
        camel_parts = re.findall(r'[A-Z]?[a-z]+|[A-Z]+(?=[A-Z]|$)', part)

        # If no camelCase split occurred, check if we need to split on number boundaries
        if not camel_parts:
            # Handle cases when numbers are not separators - keep numbers with adjacent letters
            if include_numbers and not numbers_as_separators:
                # Split on letter-number boundaries, keeping numbers with adjacent letters
                # Strategy: Match sequences of letters+digits or digits+letters, prioritizing letters+digits
                # "test2util" → ["test2", "util"] (number stays with preceding "test")
                # "123abc" → ["123abc"] (number at start stays with following "abc")
                # "test123util" → ["test123", "util"] (number stays with preceding "test")
                # Pattern explanation:
                # - \d+[a-zA-Z]+ : digits followed by letters (keeps number with following letters)
                # - [a-zA-Z]+\d+ : letters followed by digits (keeps number with preceding letters)
                # - [a-zA-Z]+ : standalone letters
                # - \d+ : standalone digits
                # Order matters: try digits+letters first to handle "123abc", then letters+digits for "test2"
                camel_parts = re.findall(r'\d+[a-zA-Z]+|[a-zA-Z]+\d+|[a-zA-Z]+|\d+', part)
            else:
                # Just use the part as-is (might be a number or single word)
                camel_parts = [part] if part else []

        for camel_part in camel_parts:
            # Normalize to lowercase
            word = camel_part.lower()
            if word:
                words.append(word)

    return words

# Maximum number of entities to analyze for naming convention discovery
# Set to 10,000 for optimal performance (~25ms) while maintaining statistical validity
MAX_ENTITIES_FOR_NAME_ANALYSIS = 10000

@mcp.tool(name="get_entity_name_parts_frequency")
def get_entity_name_parts_frequency(
    arch_name: Annotated[Optional[str], Field(description="Optional architecture longname. If not provided, analyzes all entities in the database (project-wide). Use list_architectures_summary to discover available architectures.")] = None,
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter (e.g., 'File', 'Function', 'Class').")] = None,
    name_pattern: Annotated[Optional[str], Field(description="Optional name pattern to filter entities. Only entities whose names contain this substring (case-insensitive) are included. Example: 'parser' matches 'parseMarkdown', 'cmark_parser', 'ParserTest'. Use this to discover words that co-occur with a specific pattern (e.g., 'What words appear with parser?').")] = None,
    min_word_length: Annotated[int, Field(description="Minimum word length to include. Default is 2.", ge=1, default=2)] = 2,
    recursive: Annotated[bool, Field(description="If True and arch_name is provided, includes entities from child architectures. Default is True.", default=True)] = True,
    max_results: Annotated[int, Field(description="Maximum number of words to return. Default is 50.", ge=1, le=200, default=50)] = 50,
) -> dict:
    """
    Discover naming conventions by analyzing word frequency in entity names.

    Tokenizes entity names into words (handling camelCase, snake_case, kebab-case, spaces)
    and returns the most common words/parts. Numbers are automatically removed to focus on
    semantic words (e.g., "test2util" → ["test", "util"]).

    Returns: words with frequencies, percentages, and sample entities (if compact=False),
    sorted by frequency (descending).

    Use this to discover:
    - Common prefixes/suffixes (e.g., "test", "util", "parser", "render")
    - Naming conventions (e.g., "node", "iterator", "buffer")
    - Domain-specific terminology (e.g., "markdown", "html", "git")
    - Words that co-occur with a pattern: name_pattern="parser" → discover "markdown", "html", "node"

    Examples:
    - "What naming patterns are used in this project?" → project-wide (arch_name=None)
    - "What naming patterns are used in this library?" → architecture-scoped (arch_name="...")
    - "What are the common prefixes in function names?" → kindstring="Function"
    - "What words co-occur with parser?" → name_pattern="parser"
    """
    import random
    database = require_db()

    # Get entities (architecture-scoped or project-wide)
    if arch_name:
        # Architecture-scoped analysis
        arch = database.lookup_arch(arch_name)
        if arch is None:
            return {"error": architecture_not_found_error(database, arch_name)}

        if kindstring:
            entities = expand_containers_in_architecture(arch, kindstring, recursive)
        else:
            entities = set(arch.ents(recursive))
    else:
        # Project-wide analysis
        if kindstring:
            entities = set(database.ents(kindstring))
        else:
            entities = set(database.ents())

    # Filter by name pattern early (before sampling and tokenization)
    entities = filter_entities_by_name_pattern(entities, name_pattern)

    # Sample if needed for performance (hardcoded limit for optimal performance)
    total_entities = len(entities)
    if len(entities) > MAX_ENTITIES_FOR_NAME_ANALYSIS:
        entities = random.sample(list(entities), MAX_ENTITIES_FOR_NAME_ANALYSIS)

    entities_analyzed = len(entities)
    sampled = entities_analyzed < total_entities

    # Count word frequencies
    # Use default: include_numbers=False (focus on semantic words, ignore version numbers/IDs)
    word_frequencies = {}
    word_entities = {}  # Track sample entities for each word

    for ent in entities:
        name = ent.name()
        words = tokenize_name(name, include_numbers=False, numbers_as_separators=True)

        for word in words:
            # Filter by minimum word length
            if len(word) < min_word_length:
                continue

            # Count frequency
            if word not in word_frequencies:
                word_frequencies[word] = 0
                word_entities[word] = []

            word_frequencies[word] += 1

            # Store sample entities (limit to avoid huge responses)
            if len(word_entities[word]) < 10:
                word_entities[word].append({
                    "id": ent.id(),
                    "name": ent.name(),
                    "kind": ent.kind().name(),
                    "longname": ent.longname(),
                })

    # Build results sorted by frequency (no min_frequency filter - show all results)
    result_words = []
    total_entities = len(entities)

    for word, frequency in sorted(word_frequencies.items(), key=lambda x: -x[1]):
        # Limit results
        if len(result_words) >= max_results:
            break

        percentage = (frequency / total_entities * 100) if total_entities > 0 else 0
        result_words.append({
            "word": word,
            "frequency": frequency,
            "percentage": round(percentage, 2),
            "sample_entities": word_entities[word][:5],  # Limit to 5 samples
        })

    return {
        "total_entities": total_entities,
        "entities_analyzed": entities_analyzed,
        "sampled": sampled,
        "words": result_words,
    }

@mcp.tool(name="get_architecture_dependencies")
def get_architecture_dependencies(
    arch_name: Annotated[str, Field(description="Longname of the architecture to get dependencies for. Use list_architectures_summary to discover root architecture names.")],
    forward: Annotated[bool, Field(description="If True, returns what this architecture depends on (outgoing). If False, returns what depends on this architecture (incoming). Default is True.")] = True,
    max_results: Annotated[int, Field(description="Maximum number of dependent architectures to return. Default is 50.", ge=1, le=200)] = 50,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    Get architectures that have dependencies with this architecture (layer 1 of 3).

    Returns: list of {arch_longname, reference_count} sorted by reference_count descending.

    Use forward=True to find what this architecture depends on (outgoing dependencies).
    Use forward=False to find what depends on this architecture (incoming dependencies).

    Note: Dependencies are scoped within a root architecture. For example, "Directory Structure/src"
    may depend on "Directory Structure/util", but will never report dependencies to architectures
    in different roots like "Git Author/Bob". To analyze dependencies, use architectures from the
    same root (typically "Directory Structure" for folder-based analysis).

    The reference_count indicates the strength of the dependency - use this to prioritize
    which relationships to explore further.

    Workflow:
    1. get_architecture_dependencies(arch_name) → list of dependent architectures with counts
    2. get_architecture_dependency_summary(arch_name, other_arch) → breakdown by reference kind
    3. get_architecture_dependency_references(arch_name, other_arch, ref_kindstring) → actual refs

    Supports pagination: use next_cursor from response to get more results.
    """
    database = require_db()

    arch = database.lookup_arch(arch_name)
    if arch is None:
        return {"error": architecture_not_found_error(database, arch_name)}

    if forward:
        dependencies_dict = arch.depends()
    else:
        dependencies_dict = arch.dependsby()

    dependencies = []
    for dep_arch, refs in dependencies_dict.items():
        dependencies.append({
            "arch_longname": dep_arch.longname(),
            "reference_count": len(refs),
        })

    # Sort by reference count (most dependencies first)
    dependencies.sort(key=lambda x: x["reference_count"], reverse=True)

    # Apply pagination
    paginated_deps, total_count, truncated, next_cursor = paginate_results(dependencies, max_results, cursor)

    response = {
        "dependencies": paginated_deps,
        "total_count": total_count,
        "truncated": truncated,
    }
    if next_cursor:
        response["next_cursor"] = next_cursor
    return response

@mcp.tool(name="get_architecture_dependency_summary")
def get_architecture_dependency_summary(
    arch_name: Annotated[str, Field(description="Longname of the source architecture.")],
    other_arch_name: Annotated[str, Field(description="Longname of the target architecture to get dependency details for.")],
    forward: Annotated[bool, Field(description="If True, shows references from arch_name to other_arch_name. If False, shows references from other_arch_name to arch_name. Default is True.")] = True,
) -> dict:
    """
    Get a summary of dependencies between two architectures, broken down by reference kind (layer 2 of 3).

    Returns: reference counts grouped by kind longname (e.g., {"C Call": 100, "C Use": 50, "C Include": 25}).

    The ref_counts_by_kind field uses longnames (language-qualified kind names) which can be
    used directly as filter strings in get_architecture_dependency_references. For example,
    if the summary shows "C Call": 100, you can filter with ref_kindstring="C Call" to get
    exactly those references.

    Use this after get_architecture_dependencies to understand the nature of the relationship
    between two architectures before drilling down to specific references.

    Workflow:
    1. get_architecture_dependencies(arch_name) → find "other_arch" has 500 references
    2. get_architecture_dependency_summary(arch_name, other_arch) → see it's 300 "C Call", 200 "C Use"
    3. get_architecture_dependency_references(arch_name, other_arch, "C Call") → get Call references
    """
    database = require_db()

    arch = database.lookup_arch(arch_name)
    if arch is None:
        return {"error": architecture_not_found_error(database, arch_name)}

    other_arch = database.lookup_arch(other_arch_name)
    if other_arch is None:
        return {"error": architecture_not_found_error(database, other_arch_name)}

    if forward:
        dependencies_dict = arch.depends()
    else:
        dependencies_dict = arch.dependsby()

    refs = dependencies_dict.get(other_arch, [])

    if not refs:
        return {
            "ref_counts_by_kind": {},
            "total_count": 0,
        }

    # Count references by kind (use longname for reference kinds to ensure
    # the returned strings work correctly with check() filtering)
    ref_counts_by_kind = {}
    for ref in refs:
        kind_name = ref.kind().longname()
        ref_counts_by_kind[kind_name] = ref_counts_by_kind.get(kind_name, 0) + 1

    # Sort by count descending
    sorted_counts = dict(sorted(ref_counts_by_kind.items(), key=lambda x: -x[1]))

    return {
        "ref_counts_by_kind": sorted_counts,
        "total_count": len(refs),
    }

@mcp.tool(name="get_architecture_dependency_references")
def get_architecture_dependency_references(
    arch_name: Annotated[str, Field(description="Longname of the source architecture.")],
    other_arch_name: Annotated[str, Field(description="Longname of the target architecture.")],
    forward: Annotated[bool, Field(description="If True, shows references from arch_name to other_arch_name. If False, shows references from other_arch_name to arch_name. Default is True.")] = True,
    ref_kindstring: Annotated[Optional[str], Field(description="Optional reference kind filter. Use values from get_architecture_dependency_summary output (e.g., 'C Use', 'C Set', 'C Call', 'C Include'). Generic filters like 'Call' also work but may match multiple language-specific kinds. If not provided, returns all reference kinds.")] = None,
    max_results: Annotated[int, Field(description="Maximum number of references to return. Default is 50.", ge=1, le=200)] = 50,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    Get the actual references between two architectures (layer 3 of 3).

    Returns: list of references with from_entity, to_entity, kind (longname), file, and line information.

    Use this after get_architecture_dependency_summary to see the specific references
    of a particular kind between two architectures. Use the exact kind strings from the
    summary output (e.g., "C Call", "C Use") for precise filtering.

    Workflow:
    1. get_architecture_dependencies(arch_name) → find dependent architectures
    2. get_architecture_dependency_summary(arch_name, other_arch) → see reference kinds and counts
    3. get_architecture_dependency_references(arch_name, other_arch, ref_kindstring="C Use") → get specific references

    Note: Only references where the source and target entities belong to different architectures
    are included. The reference kinds available depend on what cross-architecture references exist.
    Supports pagination: use next_cursor from response to get more results.
    """
    database = require_db()

    arch = database.lookup_arch(arch_name)
    if arch is None:
        return {"error": architecture_not_found_error(database, arch_name)}

    other_arch = database.lookup_arch(other_arch_name)
    if other_arch is None:
        return {"error": architecture_not_found_error(database, other_arch_name)}

    if forward:
        dependencies_dict = arch.depends()
    else:
        dependencies_dict = arch.dependsby()

    refs = dependencies_dict.get(other_arch, [])

    # Filter by kind if specified
    if ref_kindstring:
        refs = [ref for ref in refs if ref.kind().check(ref_kindstring)]

    # Apply pagination
    paginated_refs, total_count, truncated, next_cursor = paginate_results(refs, max_results, cursor)

    results = []
    for ref in paginated_refs:
        ent = ref.ent()
        scope = ref.scope()
        file = ref.file()

        results.append({
            "from_entity_id": scope.id() if scope else None,
            "from_entity_name": scope.longname() if scope else None,
            "to_entity_id": ent.id() if ent else None,
            "to_entity_name": ent.longname() if ent else None,
            "kind": ref.kind().longname(),
            "file": file.longname() if file else None,
            "line": ref.line(),
        })

    response = {
        "references": results,
        "total_count": total_count,
        "truncated": truncated,
    }
    if next_cursor:
        response["next_cursor"] = next_cursor
    return response

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='MCP server for Understand database')
    parser.add_argument('database', type=str, help='Path to the Understand database (.und file)')
    args = parser.parse_args()

    # Open the Understand database and store it in the module-level variable
    try:
        db = understand.open(args.database)
        if not db:
            print(f"Failed to open database: {args.database}", file=sys.stderr)
            exit(1)
        print(f"Opened Understand database: {args.database}", file=sys.stderr)
    except Exception as e:
        print(f"Error opening database: {e}", file=sys.stderr)
        exit(1)

    try:
        mcp.run(transport="stdio")
    except:
        print("Failed to run MCP server.", file=sys.stderr)
    finally:
        if db:
            db.close()
            print("Closed Understand database.", file=sys.stderr)
        print("Stopping MCP server.", file=sys.stderr)
