import argparse
import os
import sys
from typing import Annotated, List, Literal, Optional
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

@mcp.tool(name="lookup_entity_id")
def lookup_entity_id(
    name: Annotated[str, Field(description="The name or regex pattern to search for in the Understand database.")],
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string to narrow the search (e.g., 'Function', 'File', 'Class').")] = None,
    max_results: Annotated[int, Field(description="Maximum number of results to return. Default is 10 to keep responses concise.", ge=1, le=50)] = 10,
) -> List[dict]:
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
    """
    entities = require_db().lookup(name, kindstring)
    if not entities:
        return []

    # Limit results to avoid overwhelming the context window
    limited_entities = entities[:max_results]

    # Return minimal but useful information: ID, name, and kind
    results = []
    for ent in limited_entities:
        results.append({
            "id": ent.id(),
            "name": ent.name(),
            "kind": ent.kind().name(),
        })

    return results

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
    start_line: Annotated[Optional[int], Field(description="Optional start line number (1-based). If specified, only returns code from this line onwards.", ge=1)] = None,
    end_line: Annotated[Optional[int], Field(description="Optional end line number (1-based). If specified with start_line, returns only the specified line range.", ge=1)] = None,
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

    return {
        "source": source_code,
        "truncated": truncated,
        "start_line": actual_start_line,
        "end_line": actual_end_line,
        "total_lines": total_lines,
        "length": len(source_code),
    }

@mcp.tool(name="get_entity_references_summary")
def get_entity_references_summary(
    ent_id: Annotated[int, Field(description="The entity ID to get a summary of references for.", ge=1)],
) -> dict:
    """
    Get overview of all references to/from an entity (callers, callees, usage, etc.).

    Returns: total reference count, unique entities/files involved, and counts grouped by
    reference kind (e.g., 'call', 'callby', 'use', 'useby').

    Use this FIRST when exploring entity relationships to:
    - Discover what types of references exist (calls, uses, sets, etc.)
    - See reference counts before fetching details (avoids context window issues)
    - Identify which reference kinds are most common

    Workflow:
    1. get_entity_references_summary(ent_id) → see what's available
    2. get_entity_references(ent_id, refkindstring="callby") → get specific reference type
    3. Or: get_entity_references_by_file(ent_id) → see which files are involved
    """
    ent = require_db().ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")

    # Get all references
    all_refs = ent.refs()
    total_count = len(all_refs)

    # Group by reference kind
    ref_kind_counts = {}
    unique_entities = set()
    unique_files = set()

    for ref in all_refs:
        ref_kind = ref.kind().name()
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
        "note": "Use get_entity_references with a specific refkindstring to fetch detailed references for any reference kind of interest. Reference kinds are sorted by count (most common first).",
    }

@mcp.tool(name="get_entity_references_by_file")
def get_entity_references_by_file(
    ent_id: Annotated[int, Field(description="The entity ID to get references grouped by file for.", ge=1)],
    refkindstring: Annotated[Optional[str], Field(description="Optional reference kind filter string to filter references before grouping by file.")] = None,
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
                file_path = file_ent.relname() if file_ent.relname() else file_ent.longname()
                file_refs[file_id] = {
                    "file_id": file_id,
                    "file": file_path,
                    "count": 0,
                }
            file_refs[file_id]["count"] += 1

    # Sort by count (descending)
    sorted_files = sorted(file_refs.values(), key=lambda x: x["count"], reverse=True)

    return {
        "total_files": len(sorted_files),
        "total_references": len(all_refs),
        "files": sorted_files,
        "note": "Use get_entity_references with file_id parameter to fetch detailed references for a specific file.",
    }

@mcp.tool(name="get_entity_references")
def get_entity_references(
    ent_id: Annotated[int, Field(description="The entity ID to get references for.", ge=1)],
    refkindstring: Annotated[Optional[str], Field(description="Optional reference kind filter string. Use forward kinds (e.g., 'call', 'definein', 'use', 'setby') for forward references, or reverse kinds (e.g., 'callby', 'useby', 'setby') for reverse references. Multiple kinds can be comma-separated.")] = None,
    entkindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string to filter by the kind of entity being referenced (e.g., 'Function', 'Variable', 'Class'). Can be used alone without refkindstring.")] = None,
    file_id: Annotated[Optional[int], Field(description="Optional file entity ID to filter references to only those occurring in a specific file.", ge=1)] = None,
    unique: Annotated[bool, Field(description="If True, return only the first matching reference to each unique entity. Useful to avoid duplicates when the same entity is referenced multiple times. Can be used alone without other filters.")] = False,
    max_results: Annotated[int, Field(description="Maximum number of references to return. Default is 20 to keep responses concise.", ge=1, le=200)] = 20,
) -> dict:
    """
    Get detailed reference information for an entity with flexible filtering.

    Returns: list of references with referenced entity info, location (file_id, file path, line, column),
    and reference kind. Includes total_count and truncated flag.

    Use this AFTER get_entity_references_summary to fetch specific reference types:
    - Forward references: refkindstring="call" (what this calls), "use" (what this uses)
    - Reverse references: refkindstring="callby" (callers), "useby" (users)
    - Multiple kinds: refkindstring="call,callby" (comma-separated)

    Filtering options:
    - refkindstring: Filter by reference type (forward or reverse)
    - entkindstring: Filter by entity kind (e.g., only references to Functions)
    - file_id: Filter to specific file (use from get_entity_references_by_file)
    - unique: Get only first reference per unique entity (reduces duplicates)

    Each reference includes file_id so you can directly call get_entity_source without lookups.
    """
    ent = require_db().ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")

    # Get references with optional filtering
    refs = ent.refs(refkindstring=refkindstring, entkindstring=entkindstring, unique=unique)

    # Filter by file_id if specified
    if file_id is not None:
        refs = [ref for ref in refs if ref.file() and ref.file().id() == file_id]

    total_count = len(refs)

    # Limit results
    limited_refs = refs[:max_results]
    truncated = total_count > max_results

    # Build concise reference summaries
    references = []
    for ref in limited_refs:
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
            "reference_kind": ref.kind().name(),
        }

        references.append(ref_info)

    return {
        "references": references,
        "total_count": total_count,
        "returned_count": len(references),
        "truncated": truncated,
    }

@mcp.tool(name="list_metrics_summary")
def list_metrics_summary(
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string to list only metrics applicable to specific entity kinds (e.g., 'Function', 'File', 'Class').")] = None,
    include_disabled: Annotated[bool, Field(description="If True, returns all known metrics (enabled and disabled). If False (default), returns only enabled metrics. Note: metric values can be calculated even if metrics are disabled, but enabling controls visibility in the UI.")] = False,
    max_results: Annotated[int, Field(description="Maximum number of metrics to return. Default is 100 to keep responses concise. Use get_metric_details for specific metrics when descriptions are needed.", ge=1, le=500)] = 100,
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
        metric_ids = understand.Metric.list(db=database, filter=not include_disabled)
    else:
        metric_ids = understand.Metric.list(kindstring, db=database, filter=not include_disabled)

    total_count = len(metric_ids)

    # Limit results
    limited_ids = metric_ids[:max_results]
    truncated = total_count > max_results

    # Build summary with just IDs and names (no descriptions)
    metrics = []
    for metric_id in limited_ids:
        try:
            metric_name = understand.Metric.name(metric_id)
            metrics.append({
                "id": metric_id,
                "name": metric_name,
            })
        except:
            # Skip metrics that can't be retrieved
            continue

    note = "Use get_metric_details with specific metric IDs to retrieve full descriptions when needed."
    if include_disabled:
        note += " This list includes disabled metrics. Metric values can still be calculated even if disabled."
    else:
        note += " If a metric you need isn't listed, try include_disabled=True or recommend the user enable it."

    return {
        "metrics": metrics,
        "total_count": total_count,
        "returned_count": len(metrics),
        "truncated": truncated,
        "enabled_only": not include_disabled,
        "note": note,
        "documentation": {
            "built_in_metrics": "https://docs.scitools.com/metrics/dist/index.html",
            "custom_metrics": "https://support.scitools.com/support/solutions/articles/70000656986-creating-custom-metrics",
        },
    }

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
                "note": "This metric may be disabled. Metric values can still be calculated even if disabled, but consider recommending the user enable it for better visibility.",
            })

    return results

@mcp.tool(name="get_entity_metrics")
def get_entity_metrics(
    ent_id: Annotated[int, Field(description="The entity ID to get metrics for.", ge=1)],
    metric_ids: Annotated[Optional[List[str]], Field(description="Optional list of specific metric IDs to retrieve. If not provided, returns all available metrics for the entity. Note: metric values can be calculated even if the metric is disabled (not visible in UI).")] = None,
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

    if metric_ids is None:
        # Get all available metrics for this entity
        available_metric_ids = ent.metrics()
        metric_values = ent.metric(available_metric_ids)

        # Build results with names
        results = {}
        for metric_id in available_metric_ids:
            try:
                metric_name = understand.Metric.name(metric_id)
                value = metric_values.get(metric_id)
                results[metric_id] = {
                    "name": metric_name,
                    "value": value,
                }
            except:
                # Include value even if name can't be retrieved
                value = metric_values.get(metric_id)
                results[metric_id] = {
                    "value": value,
                }
    else:
        # Get specific metrics - metric() returns dict for lists
        if len(metric_ids) == 1:
            # Single metric - returns value directly
            try:
                metric_value = ent.metric(metric_ids[0])
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
            # Multiple metrics - returns dict
            metric_values = ent.metric(metric_ids)

            # Build results with names
            for metric_id in metric_ids:
                try:
                    metric_name = understand.Metric.name(metric_id)
                    value = metric_values.get(metric_id)
                    results[metric_id] = {
                        "name": metric_name,
                        "value": value,
                    }
                except:
                    # Include value even if name can't be retrieved
                    value = metric_values.get(metric_id)
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

    # Count by common kinds
    kind_counts = {}
    for ent in all_entities:
        kind = ent.kind().name()
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
    min_value: Annotated[Optional[float], Field(description="Optional minimum metric value. Entities with metric values >= min_value will be returned.")] = None,
    max_value: Annotated[Optional[float], Field(description="Optional maximum metric value. Entities with metric values <= max_value will be returned.")] = None,
    order_by: Annotated[Literal["asc", "desc"], Field(description="Sort order: 'desc' for highest values first, 'asc' for lowest values first.")] = "desc",
    max_results: Annotated[int, Field(description="Maximum number of results to return. Default is 20 to keep responses concise.", ge=1, le=200)] = 20,
) -> dict:
    """
    Find entities filtered and sorted by metric values (e.g., find high complexity functions).

    Returns: list of entities with their metric values, sorted by the metric.

    Use this to answer questions like:
    - "What are the key functions?" → Use metric_id="Cyclomatic", order_by="desc" for most complex
    - "What are the largest files?" → Use metric_id="CountLine", kindstring="File"
    - "What are the security risks?" → Use metric_id="CountInput" for functions with many inputs

    Workflow:
    1. list_metrics_summary(kindstring="Function") → discover relevant metrics
    2. find_entities_by_metric(metric_id="Cyclomatic", kindstring="Function", order_by="desc") → find top entities
    3. get_entity_details(ent_id) → get details about specific entities
    """
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

    # Limit results
    limited = entity_metrics[:max_results]

    return {
        "entities": limited,
        "total_found": len(entity_metrics),
        "returned_count": len(limited),
        "truncated": len(entity_metrics) > max_results,
        "metric_id": metric_id,
        "sort_order": order_by,
    }

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

    # Check if only basic violations exist
    only_basic_checks = len(check_ids) <= 2 and all(
        check_id in ("UND_ERROR", "UND_WARNING") for check_id in check_ids
    )

    return {
        "total_violations": total_count,
        "unique_check_ids": len(check_ids),
        "violations_by_check": {
            check_id: count for check_id, count in sorted_checks
        },
        "check_ids_list": [
            {"check_id": check_id, "count": count} for check_id, count in sorted_checks
        ],
        "note": "Use get_project_violations with a specific check_id to fetch detailed violations for that check type." if total_count > 0 else None,
        "suggestion": "No violations found or only basic UND_ERROR/UND_WARNING violations. Consider running CodeCheck with coding standards enabled or importing violations from other tools via SARIF format. See documentation links in tool description." if (total_count == 0 or only_basic_checks) else None,
    }

@mcp.tool(name="get_project_violations")
def get_project_violations(
    check_id: Annotated[Optional[str], Field(description="Optional violation check ID to filter by specific rule/check (e.g., 'MISRA_C_2012_8.4', 'UND_WARNING', 'CERT_C_2012_STR31_C'). Use get_project_violations_summary to see available check IDs.")] = None,
    file_path: Annotated[Optional[str], Field(description="Optional file path filter. Can be absolute path, relative path, or filename. Matches if the violation's file path contains this string. Note: violations store absolute file paths, and violations can exist outside of project files.")] = None,
    include_entity_info: Annotated[bool, Field(description="If True, resolves entity information for violations that have entity uniquenames. This is expensive but useful for violations like 'function too long' where you need the function entity ID. Default is False to avoid performance issues. Not all violations have entity information (some are for code regions rather than specific entities).")] = False,
    max_results: Annotated[int, Field(description="Maximum number of violations to return. Default is 50 to keep responses concise.", ge=1, le=500)] = 50,
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

    total_count = len(filtered_violations)

    # Limit results
    limited_violations = filtered_violations[:max_results]

    # Build violation summaries
    violations = []
    for viol in limited_violations:
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

    return {
        "violations": violations,
        "total_count": total_count,
        "returned_count": len(violations),
        "truncated": total_count > max_results,
    }

@mcp.tool(name="get_project_metrics")
def get_project_metrics(
    metric_ids: Annotated[Optional[List[str]], Field(description="Optional list of specific metric IDs. If not provided, returns all available project-level metrics.")] = None,
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

    if metric_ids is None:
        # Get all available project-level metrics
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
    max_results: Annotated[int, Field(description="Maximum number of entities to return. Default is 50 to keep responses concise.", ge=1, le=500)] = 50,
) -> dict:
    """
    List all entities of a specific kind in the project.

    Returns: list of entities with id, name, kind, and basic info. Useful for getting
    all functions, all classes, all files, etc.

    Use this to:
    - Get all functions in the project
    - Get all classes to understand project structure
    - Get all files for file-level analysis

    Workflow for "key functions" question:
    1. list_entities_by_kind(kindstring="Function") → get all functions
    2. find_entities_by_metric(metric_id="Cyclomatic", kindstring="Function", order_by="desc") → find most complex
    3. get_entity_details(ent_id) → get details about specific functions

    Returns minimal info (id, name, kind) to keep response concise. Use get_entity_details for more info.
    """
    database = require_db()

    # Get entities of specified kind
    entities = database.ents(kindstring)
    total_count = len(entities)

    # Limit results
    limited_entities = entities[:max_results]

    # Build concise entity summaries
    results = []
    for ent in limited_entities:
        results.append({
            "id": ent.id(),
            "name": ent.name(),
            "kind": ent.kind().name(),
            "longname": ent.longname(),
        })

    return {
        "entities": results,
        "total_count": total_count,
        "returned_count": len(results),
        "truncated": total_count > max_results,
        "kind": kindstring,
    }

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
