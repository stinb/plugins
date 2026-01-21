import argparse
import base64
import json
import os
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
    1. lookup_entity_id(name) → get list of matches
    2. Review matches and select an entity ID
    3. get_entity_details(ent_id) → get full details including location
    4. Optionally: get_entity_source(ent_id) → get source code
    5. Optionally: get_entity_references_summary(ent_id) → explore usage

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
    ent_id: Annotated[int, Field(description="The entity ID to get full details for.", ge=1)],
) -> Optional[dict]:
    """
    Get complete details about an entity including location and relationships.

    Returns: longname, kind, definition location (file path, line, column), type information,
    parent entity (if nested), and parameters (for functions/methods).

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
    ent_id: Annotated[int, Field(description="The entity ID to get source code for.", ge=1)],
    start_line: Annotated[Optional[Union[int, str]], Field(description="Optional start line number (1-based, relative to the entity's content). Line 1 is the first line of the entity. If specified, only returns code from this line onwards. Accepts numeric or string values.")] = None,
    end_line: Annotated[Optional[Union[int, str]], Field(description="Optional end line number (1-based, relative to the entity's content). If specified with start_line, returns only the specified line range. Accepts numeric or string values.")] = None,
    max_length: Annotated[int, Field(description="Maximum length of source code to return in characters. Default is 5000 to avoid filling the context window. Response will be truncated if longer.", ge=100, le=50000)] = 5000,
) -> dict:
    """
    Get source code for an entity with optional line range filtering.

    Returns: source code, truncated flag, line range returned, and total lines.

    Note: Source code is only available for entities that have contents, such as:
    - Files
    - Functions/Methods
    - Classes/Structs
    Entities like variables, namespaces that occur in multiple places, or other entities
    without a single definition location will not have source code available.

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
        raise ValueError(f"Entity with id {ent_id} does not have source code available. Source code is only available for entities like files, functions, classes that have contents.")

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
    ent_id: Annotated[int, Field(description="The entity ID to get a summary of references for.", ge=1)],
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
    ent_id: Annotated[int, Field(description="The entity ID to get references grouped by file for.", ge=1)],
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
    ent_id: Annotated[int, Field(description="The entity ID to get references for.", ge=1)],
    refkindstring: Annotated[Optional[str], Field(description="Optional reference kind filter string. Use forward kinds (e.g., 'call', 'definein', 'use', 'set') for forward references, or reverse kinds (e.g., 'callby', 'useby', 'setby') for reverse references. Multiple kinds can be comma-separated.")] = None,
    entkindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string to filter by the kind of entity being referenced (e.g., 'Function', 'Variable', 'Class'). Can be used alone without refkindstring.")] = None,
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

    Filtering options:
    - refkindstring: Filter by reference type (use values from get_entity_references_summary for precision)
    - entkindstring: Filter by entity kind (e.g., only references to Functions)
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

    Documentation:
    - Built-in metrics: https://docs.scitools.com/metrics/dist/index.html
    - Custom metric plugins: https://support.scitools.com/support/solutions/articles/70000656986-creating-custom-metrics
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

    Documentation:
    - Built-in metrics: https://docs.scitools.com/metrics/dist/index.html
    - Custom metric plugins: https://support.scitools.com/support/solutions/articles/70000656986-creating-custom-metrics
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
    ent_id: Annotated[int, Field(description="The entity ID to get metrics for.", ge=1)],
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

    Documentation:
    - Built-in metrics: https://docs.scitools.com/metrics/dist/index.html
    - Custom metric plugins: https://support.scitools.com/support/solutions/articles/70000656986-creating-custom-metrics
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
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter (e.g., 'Function', 'File', 'Class').")] = None,
    min_value: Annotated[Optional[Union[float, int, str]], Field(description="Optional minimum metric value. Entities with metric values >= min_value will be returned.")] = None,
    max_value: Annotated[Optional[Union[float, int, str]], Field(description="Optional maximum metric value. Entities with metric values <= max_value will be returned.")] = None,
    order_by: Annotated[Literal["asc", "desc"], Field(description="Sort order: 'desc' for highest values first, 'asc' for lowest values first.")] = "desc",
    max_results: Annotated[int, Field(description="Maximum number of results to return. Default is 10 to keep responses concise.", ge=1, le=200)] = 10,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    Find entities in the entire project filtered and sorted by metric values.

    Returns: list of entities with their metric values, sorted by the metric.

    Use this for project-wide queries like:
    - "What are the key functions in the project?" → metric_id="Cyclomatic", order_by="desc"
    - "What are the largest files?" → metric_id="CountLine", kindstring="File"

    For scoped queries like "key functions in src/", use find_entities_in_architecture_scope instead.

    Workflow:
    1. list_metrics_summary(kindstring="Function") → discover relevant metrics
    2. find_entities_by_metric(metric_id="Cyclomatic", kindstring="Function") → find top entities
    3. get_entity_details(ent_id) → get details about specific entities
    """
    # Coerce parameters to correct types (handle string inputs from LLMs)
    if min_value is not None:
        min_value = float(min_value)
    if max_value is not None:
        max_value = float(max_value)

    database = require_db()

    # Get entities of specified kind
    if kindstring:
        entities = database.ents(kindstring)
    else:
        entities = database.ents()

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
        "metric_id": metric_id,
        "sort_order": order_by,
    }
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

    For CodeCheck standards and instructions, see: https://docs.scitools.com and
    https://support.scitools.com/support/solutions/articles/70000583282-codecheck-overview

    For importing SARIF files, see: https://support.scitools.com/support/solutions/articles/70000641310-importing-sarif-files-in-understand
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

@mcp.tool(name="list_entities_by_kind")
def list_entities_by_kind(
    kindstring: Annotated[str, Field(description="Entity kind filter string (e.g., 'Function', 'File', 'Class', 'Method').")],
    max_results: Annotated[int, Field(description="Maximum number of entities to return. Default is 20 to keep responses concise.", ge=1, le=500)] = 20,
    compact: Annotated[bool, Field(description="If True (default), returns id, name, kind only. If False, includes longname.")] = True,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    List all entities of a specific kind in the entire project.

    Returns: list of entities with id, name, kind, and basic info.

    Use this for project-wide listing:
    - Get all functions in the project
    - Get all classes to understand project structure
    - Get all files for file-level analysis

    For scoped queries like "functions in src/", use find_entities_in_architecture_scope instead.
    For finding entities by metric values (e.g., "most complex functions"), use find_entities_by_metric instead.

    Workflow:
    1. list_entities_by_kind(kindstring="Function") → get all functions
    2. get_entity_details(ent_id) → get details about specific entities
    3. get_entity_source(ent_id) → read source code

    Returns minimal info (id, name, kind) to keep response concise. Use get_entity_details for more info.
    Supports pagination: use next_cursor from response to get more results.

    Filter examples: "function ~unresolved" (defined functions only), "file ~header" (source files only).
    """
    database = require_db()

    # Get entities of specified kind
    entities = database.ents(kindstring)

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
        "kind": kindstring,
    }
    if next_cursor:
        response["next_cursor"] = next_cursor
    return response

@mcp.tool(name="list_architectures_summary")
def list_architectures_summary() -> dict:
    """
    List root architectures in the project.

    Returns: list of root architecture names. Most projects have only 1-3 root architectures.

    All projects with at least one file will have a "Directory Structure" architecture showing
    the folder structure. Additional architectures may be created manually or via plugins.

    Common architectures:
    - "Directory Structure" - built-in folder hierarchy (always present)
    - Custom architectures - created by users or plugins (e.g., functional decomposition, git metadata)

    Workflow:
    1. list_architectures_summary() → discover available architectures
    2. get_architecture_details(arch_name) → get hierarchy and entity information
    3. get_entities_in_architecture(arch_name) → explore files/entities in an architecture
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

    After getting the longname, use get_architecture_details or get_entities_in_architecture to explore.
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

@mcp.tool(name="get_architecture_details")
def get_architecture_details(
    arch_name: Annotated[str, Field(description="Longname of the architecture to get details for. Use list_architectures_summary to discover root architecture names.")],
) -> dict:
    """
    Get detailed information about a specific architecture including entity counts by kind.

    Returns: architecture details including longname, children count, total entity count,
    and entity_counts_by_kind (breakdown of direct members by kind, e.g., File, Class, Function).

    Note: entity_counts_by_kind shows only direct members. For Directory Structure, this means
    files only (not functions inside those files). To find nested entities like functions within
    a folder, use find_entities_in_architecture_scope.

    Use this to understand the structure and contents of an architecture:
    - See how many child architectures exist
    - Understand what types of entities are directly assigned

    Workflow for exploring architecture hierarchy:
    1. list_architectures_summary() → discover root architectures
    2. get_architecture_details(arch_name) → see child count and entity breakdown
    3. get_architecture_children(arch_name) → navigate deeper into hierarchy
    4. get_entities_in_architecture(arch_name) → get direct members

    Workflow for "key functions in src/":
    1. get_architecture_details("Directory Structure/src") → see it contains files
    2. find_entities_in_architecture_scope("Directory Structure/src", kindstring="Function", metric_id="Cyclomatic") → find functions

    Note: The longname format is "parent/child" or just "name" for root architectures.
    """
    database = require_db()

    arch = database.lookup_arch(arch_name)
    if arch is None:
        return {"error": f"Architecture '{arch_name}' not found. Use list_architectures_summary to discover available architectures."}

    children = arch.children()
    entities = arch.ents(False)

    # Count entities by kind (use filterable kind names to ensure
    # the returned strings work correctly with kindstring filters)
    entity_counts_by_kind = {}
    for ent in entities:
        kind_name = get_filterable_kind_name(ent)
        entity_counts_by_kind[kind_name] = entity_counts_by_kind.get(kind_name, 0) + 1

    # Sort by count descending
    sorted_counts = dict(sorted(entity_counts_by_kind.items(), key=lambda x: -x[1]))

    return {
        "longname": arch.longname(),
        "child_count": len(children),
        "entity_count": len(entities),
        "entity_counts_by_kind": sorted_counts,
        "is_root": arch.parent() is None,
    }

@mcp.tool(name="get_architectures_for_entity")
def get_architectures_for_entity(
    ent_id: Annotated[int, Field(description="The entity ID to find architectures for.", ge=1)],
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

@mcp.tool(name="get_entities_in_architecture")
def get_entities_in_architecture(
    arch_name: Annotated[str, Field(description="Longname of the architecture to get entities from. Use list_architectures_summary to discover root architecture names.")],
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter (e.g., 'File', 'Function', 'Class') to filter entities by kind.")] = None,
    max_results: Annotated[int, Field(description="Maximum number of entities to return. Default is 20.", ge=1, le=500)] = 20,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    Get direct members of an architecture (no expansion of containers).

    Returns: list of entities with id, name, kind, and longname.

    Returns only entities directly assigned to the architecture:
    - Directory Structure architectures contain files (not the functions inside them)
    - User-created architectures contain whatever entities the user assigned

    Use this to see what's directly in an architecture:
    - List files in a folder (Directory Structure)
    - See entities in a user-created grouping (e.g., "Security Critical" architecture)
    - Find files by metadata (Calendar, Git Author architectures)

    For finding nested entities like "functions in a folder", use find_entities_in_architecture_scope
    instead, which expands files/classes to find contained entities.

    Workflow for exploring architectures:
    1. list_architectures_summary() → discover root architectures
    2. get_architecture_children(arch_name) → navigate to specific folder
    3. get_entities_in_architecture(arch_name) → see files/entities in that folder
    4. get_entity_details(ent_id) or get_entity_source(ent_id) → examine specific entities

    Supports pagination: use next_cursor from response to get more results.
    """
    database = require_db()

    arch = database.lookup_arch(arch_name)
    if arch is None:
        return {"error": f"Architecture '{arch_name}' not found. Use list_architectures_summary to discover available architectures."}

    entities = arch.ents(False)

    # Filter by kind if specified
    if kindstring:
        entities = [ent for ent in entities if ent.kind().check(kindstring)]

    # Apply pagination
    paginated_entities, total_count, truncated, next_cursor = paginate_results(entities, max_results, cursor)

    results = []
    for ent in paginated_entities:
        results.append({
            "id": ent.id(),
            "name": ent.name(),
            "kind": ent.kind().name(),
            "longname": ent.longname(),
        })

    response = {
        "entities": results,
        "total_count": total_count,
        "truncated": truncated,
    }
    if next_cursor:
        response["next_cursor"] = next_cursor
    return response

@mcp.tool(name="find_entities_in_architecture_scope")
def find_entities_in_architecture_scope(
    arch_name: Annotated[str, Field(description="Longname of the architecture to search within.")],
    kindstring: Annotated[str, Field(description="Entity kind to find (e.g., 'Function', 'Class', 'Method'). Required.")],
    metric_id: Annotated[Optional[str], Field(description="Optional metric ID to sort/filter by (e.g., 'Cyclomatic', 'CountLine').")] = None,
    min_value: Annotated[Optional[Union[float, int, str]], Field(description="Optional minimum metric value filter.")] = None,
    max_value: Annotated[Optional[Union[float, int, str]], Field(description="Optional maximum metric value filter.")] = None,
    order_by: Annotated[Optional[Literal["asc", "desc"]], Field(description="Sort order when metric_id is provided: 'desc' for highest first, 'asc' for lowest first.")] = None,
    max_results: Annotated[int, Field(description="Maximum number of entities to return. Default is 20.", ge=1, le=500)] = 20,
    cursor: Annotated[Optional[str], Field(description="Pagination cursor from previous response to get next page of results.")] = None,
) -> dict:
    """
    Find entities within an architecture's logical scope, expanding containers as needed.

    Unlike get_entities_in_architecture (which returns direct members only), this tool
    finds entities that are *logically within* an architecture by expanding container
    entities (files, classes, packages) to find nested entities (functions, methods).

    Use this to answer questions like:
    - "What are the most complex functions in the src folder?" → metric_id="Cyclomatic", order_by="desc"
    - "What classes are in the utils directory?" → kindstring="Class"
    - "Find the largest methods in this module" → kindstring="Method", metric_id="CountLine"

    For architectures that directly contain the target kind (e.g., a user-created
    "Key Functions" architecture), this behaves like get_entities_in_architecture.

    Workflow:
    1. get_architecture_details(arch_name) → see what's in the architecture
    2. find_entities_in_architecture_scope(arch_name, kindstring="Function", metric_id="Cyclomatic") → find key functions
    3. get_entity_details(ent_id) → get details about specific entities
    """
    # Coerce parameters to correct types (handle string inputs from LLMs)
    if min_value is not None:
        min_value = float(min_value)
    if max_value is not None:
        max_value = float(max_value)

    database = require_db()

    arch = database.lookup_arch(arch_name)
    if arch is None:
        return {"error": f"Architecture '{arch_name}' not found. Use list_architectures_summary to discover available architectures."}

    # Get direct entities from architecture
    direct_entities = arch.ents(False)

    # Collect entities matching the kindstring, expanding containers as needed
    matching_entities = set()
    files_to_expand = set()
    classes_to_expand = set()
    packages_to_expand = set()

    for ent in direct_entities:
        if ent.kind().check(kindstring):
            # Direct match
            matching_entities.add(ent)
        elif ent.kind().check("file ~unresolved ~unknown"):
            files_to_expand.add(ent)
        elif ent.kind().check("python package ~unknown"):
            packages_to_expand.add(ent)
        elif ent.kind().check("class ~unresolved, struct ~unresolved"):
            classes_to_expand.add(ent)

    # Expand Python packages to get their files
    for pkg in packages_to_expand:
        for ref in pkg.refs("python contain", "file ~unknown ~unresolved", True):
            file_ent = ref.ent()
            if file_ent.kind().check(kindstring):
                matching_entities.add(file_ent)
            else:
                files_to_expand.add(file_ent)

    # Expand files to find nested entities
    for file_ent in files_to_expand:
        for ref in file_ent.filerefs("define, declare", kindstring, True):
            matching_entities.add(ref.ent())

    # Expand classes to find nested entities (methods, nested classes)
    for class_ent in classes_to_expand:
        for ref in class_ent.refs("define, declare", kindstring, True):
            matching_entities.add(ref.ent())

    # Convert to list for processing
    entities_list = list(matching_entities)

    # If metric_id is provided, get metric values and sort/filter
    if metric_id:
        entity_metrics = []
        for ent in entities_list:
            try:
                value = ent.metric(metric_id)
                if value is not None:
                    # Apply min/max filters
                    if min_value is not None and value < min_value:
                        continue
                    if max_value is not None and value > max_value:
                        continue
                    entity_metrics.append((ent, value))
            except:
                continue

        # Sort by metric value if order_by is specified
        if order_by:
            reverse = (order_by == "desc")
            entity_metrics.sort(key=lambda x: x[1], reverse=reverse)

        # Apply pagination
        paginated, total_count, truncated, next_cursor = paginate_results(entity_metrics, max_results, cursor)

        # Build results with metric values
        results = []
        for ent, value in paginated:
            results.append({
                "id": ent.id(),
                "name": ent.name(),
                "kind": ent.kind().name(),
                "longname": ent.longname(),
                "metric_value": value,
            })

        response = {
            "entities": results,
            "total_count": total_count,
            "truncated": truncated,
            "metric_id": metric_id,
        }
    else:
        # No metric - just return entities
        # Apply pagination
        paginated, total_count, truncated, next_cursor = paginate_results(entities_list, max_results, cursor)

        results = []
        for ent in paginated:
            results.append({
                "id": ent.id(),
                "name": ent.name(),
                "kind": ent.kind().name(),
                "longname": ent.longname(),
            })

        response = {
            "entities": results,
            "total_count": total_count,
            "truncated": truncated,
        }

    if next_cursor:
        response["next_cursor"] = next_cursor
    return response


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
    4. get_entities_in_architecture(arch_name="Directory Structure/src") → files in that folder

    Supports pagination: use next_cursor from response to get more results.
    """
    database = require_db()

    arch = database.lookup_arch(arch_name)
    if arch is None:
        return {"error": f"Architecture '{arch_name}' not found. Use list_architectures_summary to discover available architectures."}

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
        return {"error": f"Architecture '{arch_name}' not found. Use list_architectures_summary to discover available architectures."}

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
        return {"error": f"Architecture '{arch_name}' not found. Use list_architectures_summary to discover available architectures."}

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
        return {"error": f"Architecture '{arch_name}' not found."}

    other_arch = database.lookup_arch(other_arch_name)
    if other_arch is None:
        return {"error": f"Architecture '{other_arch_name}' not found."}

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
        return {"error": f"Architecture '{arch_name}' not found."}

    other_arch = database.lookup_arch(other_arch_name)
    if other_arch is None:
        return {"error": f"Architecture '{other_arch_name}' not found."}

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
