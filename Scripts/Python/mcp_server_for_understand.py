import argparse
import os
import sys
from dataclasses import dataclass, asdict
from typing import List, Optional, Union

# Add DLL directory if specified in environment variable (for Windows)
if "UNDERSTAND_DLL_DIR" in os.environ:
    os.add_dll_directory(os.environ["UNDERSTAND_DLL_DIR"])

import understand
from mcp.server.fastmcp import FastMCP

@dataclass
class Entity:
    """Data class representing an Understand entity."""
    id: int
    longname: str
    longkind: str
    uniquename: str

@dataclass
class Ref:
    """Data class representing an Understand reference."""
    column: int
    ent_id: int
    file_id: int
    isforward: bool
    longkind: str
    line: int
    scope_id: int

@dataclass
class Ignore:
    """Data class representing an ignore entry for a violation."""
    source: str
    note: str

@dataclass
class Violation:
    """Data class representing an Understand violation."""
    check_id: str
    column: int
    ent_uniquename: str
    file: str
    ignores: List[Ignore]
    line: int
    text: str

@dataclass
class Atn:
    """Data class representing an Understand annotation."""
    author: str
    date: str
    ent_id: Optional[int]
    line: int
    text: str

@dataclass
class Metric:
    """Data class representing an Understand metric."""
    id: str
    name: str
    description: str

@dataclass
class Dependency:
    """Data class representing a dependency relationship."""
    ent_id: int
    refs: List[Ref]

@dataclass
class ArchDependency:
    """Data class representing an architecture dependency relationship."""
    arch_longname: str
    refs: List[Ref]

@dataclass
class Lexeme:
    """Data class representing an Understand lexeme."""
    column_begin: int
    column_end: int
    ent_id: Optional[int]
    inactive: bool
    line_begin: int
    line_end: int
    ref: Optional[Ref]
    text: str
    token: str

@dataclass
class CFNodeChild:
    """Data class representing a child node in a control flow graph."""
    id: int
    label: Optional[str]

@dataclass
class CFNode:
    """Data class representing an Understand control flow node."""
    id: int
    kind: str
    line_begin: Optional[int]
    column_begin: Optional[int]
    line_end: Optional[int]
    column_end: Optional[int]
    children: List[CFNodeChild]
    end_node: Optional[int]

# Define MCP server instance
mcp = FastMCP("UNDERSTAND_MCP")

# Global variable to store the opened database
db = None

def ent_to_entity(ent: understand.Ent) -> Entity:
    """Convert an Understand entity to an Entity dataclass."""
    return Entity(
        id=ent.id(),
        longname=ent.longname(),
        longkind=ent.kind().longname(),
        uniquename=ent.uniquename()
    )

def ref_to_ref(ref: understand.Ref) -> Ref:
    """Convert an Understand reference to a Ref dataclass."""
    return Ref(
        column=ref.column(),
        ent_id=ref.ent().id(),
        file_id=ref.file().id(),
        isforward=ref.isforward(),
        longkind=ref.kind().longname(),
        line=ref.line(),
        scope_id=ref.scope().id()
    )

def violation_to_violation(viol: understand.Violation) -> Violation:
    """Convert an Understand violation to a Violation dataclass."""
    ignore_list = [Ignore(source=source, note=note) for source, note in viol.ignores()]
    return Violation(
        check_id=viol.check_id(),
        column=viol.column(),
        ent_uniquename=viol.ent_uniquename(),
        file=viol.file(),
        ignores=ignore_list,
        line=viol.line(),
        text=viol.text()
    )

def atn_to_atn(atn: understand.Atn) -> Atn:
    """Convert an Understand annotation to an Atn dataclass."""
    ent = atn.ent()
    ent_id = ent.id() if ent is not None else None
    return Atn(
        author=atn.author(),
        date=atn.date(),
        ent_id=ent_id,
        line=atn.line(),
        text=atn.text()
    )

def metric_id_to_metric(metric_id: str) -> Metric:
    """Convert a metric ID to a Metric dataclass."""
    return Metric(
        id=metric_id,
        name=understand.Metric.name(metric_id),
        description=understand.Metric.description(metric_id)
    )

def lexeme_to_lexeme(lexeme: understand.Lexeme) -> Lexeme:
    """Convert an Understand lexeme to a Lexeme dataclass."""
    ent = lexeme.ent()
    ent_id = ent.id() if ent is not None else None
    ref = lexeme.ref()
    ref_obj = ref_to_ref(ref) if ref is not None else None
    return Lexeme(
        column_begin=lexeme.column_begin(),
        column_end=lexeme.column_end(),
        ent_id=ent_id,
        inactive=lexeme.inactive(),
        line_begin=lexeme.line_begin(),
        line_end=lexeme.line_end(),
        ref=ref_obj,
        text=lexeme.text(),
        token=lexeme.token()
    )

def cfnode_to_cfnode(node: understand.CFNode, node_list: List[understand.CFNode]) -> CFNode:
    """Convert an Understand CFNode to a CFNode dataclass."""
    # Find the index of this node in the list
    node_id = node_list.index(node)
    
    # Convert children to CFNodeChild objects with their IDs and labels
    children_list = []
    for child in node.children():
        child_id = node_list.index(child)
        label = node.child_label(child)
        children_list.append(CFNodeChild(id=child_id, label=label))
    
    # Find the index of the end_node if it exists
    end_node = node.end_node()
    end_node_id = None
    if end_node is not None:
        end_node_id = node_list.index(end_node)
    
    return CFNode(
        id=node_id,
        kind=node.kind(),
        line_begin=node.line_begin(),
        column_begin=node.column_begin(),
        line_end=node.line_end(),
        column_end=node.column_end(),
        children=children_list,
        end_node=end_node_id
    )

@mcp.tool(
    name = "understand_version",
    description = understand.version.__doc__)

def understand_version() -> str:
    return understand.version()

@mcp.tool(
    name = "lookup",
    description = """
    Look up entities by name in the Understand database.
        Arguments:
            name: The name to search for (can be a string or regex pattern).
            kindstring: Optional entity kind filter string (e.g., "Function", "File").
        Returns:
            A list of Entity objects matching the name.
    """)

def lookup(name: str, kindstring: Optional[str] = None) -> List[dict]:
    """Look up entities by name."""
    if db is None:
        raise RuntimeError("Database is not open")
    
    entities = db.lookup(name, kindstring)
    return [asdict(ent_to_entity(ent)) for ent in entities]

@mcp.tool(
    name = "lookup_uniquename",
    description = """
    Look up an entity by its unique name in the Understand database.
        Arguments:
            uniquename: The unique name of the entity.
        Returns:
            An Entity object if found, or None if not found.
    """)

def lookup_uniquename(uniquename: str) -> Optional[dict]:
    """Look up an entity by unique name."""
    if db is None:
        raise RuntimeError("Database is not open")
    
    ent = db.lookup_uniquename(uniquename)
    if ent is None:
        return None
    return asdict(ent_to_entity(ent))

@mcp.tool(
    name = "ents",
    description = """
    Get entities from the Understand database, optionally filtered by kind.
        Arguments:
            kindstring: Optional entity kind filter string (e.g., "Function", "File", "Class").
        Returns:
            A list of Entity objects matching the filter, or all entities if no filter is specified.
    """)

def ents(kindstring: Optional[str] = None) -> List[dict]:
    """Get entities from the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    
    entities = db.ents(kindstring)
    return [asdict(ent_to_entity(ent)) for ent in entities]

@mcp.tool(
    name = "db_ent_from_id",
    description = """
    Get an entity by its ID.
        Arguments:
            ent_id: The entity ID.
        Returns:
            An Entity object, or None if not found.
    """)

def db_ent_from_id(ent_id: int) -> Optional[dict]:
    """Get an entity by its ID."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        return None
    return asdict(ent_to_entity(ent))

@mcp.tool(
    name = "db_files",
    description = """
    Get all file entities in the database.
        Arguments:
            None
        Returns:
            A list of Entity objects representing file entities.
    """)

def db_files() -> List[dict]:
    """Get all file entities in the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    files = db.files()
    return [asdict(ent_to_entity(ent)) for ent in files]

@mcp.tool(
    name = "db_language",
    description = """
    Get the project languages.
        Arguments:
            None
        Returns:
            A tuple of language strings enabled in the project.
    """)

def db_language() -> tuple:
    """Get the project languages."""
    if db is None:
        raise RuntimeError("Database is not open")
    return db.language()

@mcp.tool(
    name = "db_metric",
    description = """
    Get metric value(s) for the database.
        Arguments:
            metric: A metric name, or a list of metric names.
            metric_format: Optional format string ("auto", "raw", or "string", default "auto").
        Returns:
            A dictionary of metric values if metric is a list, or a single metric value if metric is a string.
    """)

def db_metric(metric: Union[str, List[str]], metric_format: Optional[str] = None):
    """Get metric value(s) for the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    if metric_format is None:
        return db.metric(metric)
    else:
        return db.metric(metric, metric_format)

@mcp.tool(
    name = "db_name",
    description = """
    Get the filename of the database.
        Arguments:
            None
        Returns:
            The filename of the database as a string.
    """)

def db_name() -> str:
    """Get the filename of the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    return db.name()

@mcp.tool(
    name = "ent_comments",
    description = """
    Get comments associated with an entity.
        Arguments:
            ent_id: The entity ID.
            style: Optional comment location ("before" or "after").
            raw: Optional boolean to preserve original formatting (default False).
            refkindstring: Optional reference kind string.
        Returns:
            Comments as a string or list of strings if raw=True.
    """)

def ent_comments(ent_id: int, style: Optional[str] = None, raw: Optional[bool] = None, refkindstring: Optional[str] = None):
    """Get comments for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.comments(style, raw, refkindstring)

@mcp.tool(
    name = "ent_contents",
    description = """
    Get the contents of an entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            The entity's contents as a string.
    """)

def ent_contents(ent_id: int) -> str:
    """Get contents for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.contents()

@mcp.tool(
    name = "ent_ents",
    description = """
    Get entities that reference or are referenced by an entity.
        Arguments:
            ent_id: The entity ID.
            refkindstring: Optional reference kind filter string. If not provided, all references are returned.
            entkindstring: Optional entity kind filter string.
        Returns:
            A list of Entity objects.
    """)

def ent_ents(ent_id: int, refkindstring: Optional[str] = None, entkindstring: Optional[str] = None) -> List[dict]:
    """Get referenced entities for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    if refkindstring is None:
        entities = ent.ents()
    elif entkindstring is None:
        entities = ent.ents(refkindstring)
    else:
        entities = ent.ents(refkindstring, entkindstring)
    return [asdict(ent_to_entity(e)) for e in entities]

@mcp.tool(
    name = "ent_file_type",
    description = """
    Get the file type of an entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            The file type as a string.
    """)

def ent_file_type(ent_id: int) -> str:
    """Get file type for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.file_type()

@mcp.tool(
    name = "ent_freetext",
    description = """
    Get extra parser information for an entity.
        Arguments:
            ent_id: The entity ID.
            option: The type of information to retrieve (e.g., "AllowExceptions", "Inline", "Noexcept").
        Returns:
            The value stored by the parser (string or bool).
    """)

def ent_freetext(ent_id: int, option: str):
    """Get freetext for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.freetext(option)

@mcp.tool(
    name = "ent_ib",
    description = """
    Get Info Browser information for an entity.
        Arguments:
            ent_id: The entity ID.
            options: Optional formatting options string.
        Returns:
            A list of strings containing Info Browser information.
    """)

def ent_ib(ent_id: int, options: Optional[str] = None) -> List[str]:
    """Get Info Browser information for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.ib(options)

@mcp.tool(
    name = "ent_kindname",
    description = """
    Get the simple kind name for an entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            The simple kind name as a string.
    """)

def ent_kindname(ent_id: int) -> str:
    """Get kindname for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.kindname()

@mcp.tool(
    name = "ent_language",
    description = """
    Get the language of an entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            The language as a string.
    """)

def ent_language(ent_id: int) -> str:
    """Get language for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.language()

@mcp.tool(
    name = "ent_library",
    description = """
    Get the library an entity belongs to.
        Arguments:
            ent_id: The entity ID.
        Returns:
            The library name as a string (empty string if not in a library).
    """)

def ent_library(ent_id: int) -> str:
    """Get library for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.library()

@mcp.tool(
    name = "ent_longname",
    description = """
    Get the long name of an entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            The long name as a string.
    """)

def ent_longname(ent_id: int) -> str:
    """Get longname for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.longname()

@mcp.tool(
    name = "ent_name",
    description = """
    Get the short name of an entity.
        Arguments:
            ent_id: The entity ID.
            template_args: Optional boolean to include template instantiation arguments (default False).
        Returns:
            The short name as a string.
    """)

def ent_name(ent_id: int, template_args: Optional[bool] = None) -> str:
    """Get name for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.name(template_args)

@mcp.tool(
    name = "ent_parameters",
    description = """
    Get the parameters for an entity.
        Arguments:
            ent_id: The entity ID.
            shownames: Optional boolean to include parameter names (default True).
        Returns:
            Parameters as a string, or None if not available.
    """)

def ent_parameters(ent_id: int, shownames: Optional[bool] = None) -> Optional[str]:
    """Get parameters for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.parameters(shownames)

@mcp.tool(
    name = "ent_parent",
    description = """
    Get the parent entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            The parent Entity object, or None if no parent exists.
    """)

def ent_parent(ent_id: int) -> Optional[dict]:
    """Get parent for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    parent = ent.parent()
    if parent is None:
        return None
    return asdict(ent_to_entity(parent))

@mcp.tool(
    name = "ent_parsetime",
    description = """
    Get the last parse time for a file entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            The parse time as Unix/Posix timestamp (0 if not a parse file).
    """)

def ent_parsetime(ent_id: int) -> int:
    """Get parsetime for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.parsetime()

@mcp.tool(
    name = "ent_relname",
    description = """
    Get the relative name of a file entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            The relative name as a string, or None for non-file entities.
    """)

def ent_relname(ent_id: int) -> Optional[str]:
    """Get relname for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.relname()

@mcp.tool(
    name = "ent_simplename",
    description = """
    Get the simple name of an entity (deprecated, equivalent to name).
        Arguments:
            ent_id: The entity ID.
        Returns:
            The simple name as a string.
    """)

def ent_simplename(ent_id: int) -> str:
    """Get simplename for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.simplename()

@mcp.tool(
    name = "ent_type",
    description = """
    Get the type string of an entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            The type as a string.
    """)

def ent_type(ent_id: int) -> str:
    """Get type for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.type()

@mcp.tool(
    name = "ent_uniquename",
    description = """
    Get the unique name of an entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            The unique name as a string.
    """)

def ent_uniquename(ent_id: int) -> str:
    """Get uniquename for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.uniquename()

@mcp.tool(
    name = "ent_value",
    description = """
    Get the value associated with an entity (for enumerators, initialized variables, macros).
        Arguments:
            ent_id: The entity ID.
        Returns:
            The value as a string.
    """)

def ent_value(ent_id: int) -> str:
    """Get value for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.value()

@mcp.tool(
    name = "ent_violations",
    description = """
    Get violations for a file entity.
        Arguments:
            ent_id: The entity ID (must be a file entity).
        Returns:
            A list of Violation objects, or None for non-file entities.
    """)

def ent_violations(ent_id: int) -> Optional[List[dict]]:
    """Get violations for a file entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    violations = ent.violations()
    if violations is None:
        return None
    return [asdict(violation_to_violation(viol)) for viol in violations]

@mcp.tool(
    name = "db_violations",
    description = """
    Get all violations in the database.
        Arguments:
            None
        Returns:
            A list of Violation objects for all violations in the database.
    """)

def db_violations() -> List[dict]:
    """Get all violations in the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    violations = db.violations()
    return [asdict(violation_to_violation(viol)) for viol in violations]

@mcp.tool(
    name = "ent_annotations",
    description = """
    Get annotations for an entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            A list of Atn objects associated with the entity.
    """)

def ent_annotations(ent_id: int) -> List[dict]:
    """Get annotations for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    annotations = ent.annotations()
    return [asdict(atn_to_atn(atn)) for atn in annotations]

@mcp.tool(
    name = "db_annotations",
    description = """
    Get all annotations in the database.
        Arguments:
            None
        Returns:
            A list of Atn objects for all annotations in the database.
    """)

def db_annotations() -> List[dict]:
    """Get all annotations in the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    annotations = db.annotations()
    return [asdict(atn_to_atn(atn)) for atn in annotations]

@mcp.tool(
    name = "metric_list",
    description = """
    List available metrics using understand.Metric.list.
        Arguments:
            kindstring: Optional entity kind filter string.
            filter_enabled: Optional boolean to filter to only enabled metrics (default True).
        Returns:
            A list of Metric objects.
    """)

def metric_list(kindstring: Optional[str] = None, filter_enabled: Optional[bool] = None) -> List[dict]:
    """List available metrics."""
    if db is None:
        raise RuntimeError("Database is not open")
    
    # Handle different parameter combinations for understand.Metric.list
    if kindstring is None:
        if filter_enabled is None:
            metric_ids = understand.Metric.list(db=db)
        else:
            metric_ids = understand.Metric.list(db=db, filter=filter_enabled)
    else:
        if filter_enabled is None:
            metric_ids = understand.Metric.list(kindstring, db=db)
        else:
            metric_ids = understand.Metric.list(kindstring, db=db, filter=filter_enabled)
    
    return [asdict(metric_id_to_metric(metric_id)) for metric_id in metric_ids]

@mcp.tool(
    name = "db_metrics",
    description = """
    Get project level metrics from the database.
        Arguments:
            None
        Returns:
            A list of Metric objects for project level metrics.
    """)

def db_metrics() -> List[dict]:
    """Get project level metrics from the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    metric_ids = db.metrics()
    return [asdict(metric_id_to_metric(metric_id)) for metric_id in metric_ids]

@mcp.tool(
    name = "ent_metrics",
    description = """
    Get metrics defined for an entity.
        Arguments:
            ent_id: The entity ID.
        Returns:
            A list of Metric objects for metrics defined for the entity.
    """)

def ent_metrics(ent_id: int) -> List[dict]:
    """Get metrics for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    metric_ids = ent.metrics()
    return [asdict(metric_id_to_metric(metric_id)) for metric_id in metric_ids]

@mcp.tool(
    name = "ent_metric",
    description = """
    Get metric value(s) for an entity.
        Arguments:
            ent_id: The entity ID.
            metric: A metric name, or a list of metric names.
            metric_format: Optional format string ("auto", "raw", or "string", default "auto").
        Returns:
            A dictionary of metric values if metric is a list, or a single metric value if metric is a string.
    """)

def ent_metric(ent_id: int, metric: Union[str, List[str]], metric_format: Optional[str] = None):
    """Get metric value(s) for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    if metric_format is None:
        return ent.metric(metric)
    else:
        return ent.metric(metric, metric_format)

@mcp.tool(
    name = "ent_kind_check",
    description = """
    Check if an entity's kind matches a filter string.
        Arguments:
            ent_id: The entity ID.
            kindstring: A kind filter string to match against.
        Returns:
            True if the entity's kind matches the filter string, False otherwise.
    """)

def ent_kind_check(ent_id: int, kindstring: str) -> bool:
    """Check if an entity's kind matches a filter string."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.kind().check(kindstring)

@mcp.tool(
    name = "ent_depends",
    description = """
    Get dependencies for a class or file entity.
        Arguments:
            ent_id: The entity ID (must be a class or file entity).
        Returns:
            A list of Dependency objects representing entities this entity depends on.
    """)

def ent_depends(ent_id: int) -> List[dict]:
    """Get dependencies for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    dependencies_dict = ent.depends()
    dependencies = []
    for dep_ent, refs in dependencies_dict.items():
        ref_list = [ref_to_ref(ref) for ref in refs]
        dependencies.append(Dependency(ent_id=dep_ent.id(), refs=ref_list))
    return [asdict(dep) for dep in dependencies]

@mcp.tool(
    name = "ent_dependsby",
    description = """
    Get entities that depend on this class or file entity.
        Arguments:
            ent_id: The entity ID (must be a class or file entity).
        Returns:
            A list of Dependency objects representing entities that depend on this entity.
    """)

def ent_dependsby(ent_id: int) -> List[dict]:
    """Get entities that depend on this entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    dependencies_dict = ent.dependsby()
    dependencies = []
    for dep_ent, refs in dependencies_dict.items():
        ref_list = [ref_to_ref(ref) for ref in refs]
        dependencies.append(Dependency(ent_id=dep_ent.id(), refs=ref_list))
    return [asdict(dep) for dep in dependencies]

@mcp.tool(
    name = "db_root_archs",
    description = """
    Get root architectures for the database.
        Arguments:
            None
        Returns:
            A list of architecture longnames (strings).
    """)

def db_root_archs() -> List[str]:
    """Get root architectures for the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    archs = db.root_archs()
    return [arch.longname() for arch in archs]

@mcp.tool(
    name = "db_archs",
    description = """
    Get architectures that contain an entity.
        Arguments:
            ent_id: The entity ID.
            implicit: Optional boolean to return architectures that contain any of the entity's parents (default False).
        Returns:
            A list of architecture longnames (strings).
    """)

def db_archs(ent_id: int, implicit: Optional[bool] = None) -> List[str]:
    """Get architectures that contain an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    if implicit is None:
        archs = db.archs(ent)
    else:
        archs = db.archs(ent, implicit)
    return [arch.longname() for arch in archs]

@mcp.tool(
    name = "arch_children",
    description = """
    Get children of an architecture.
        Arguments:
            arch_longname: The long name of the architecture.
        Returns:
            A list of child architecture longnames (strings).
    """)

def arch_children(arch_longname: str) -> List[str]:
    """Get children of an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    children = arch.children()
    return [child.longname() for child in children]

@mcp.tool(
    name = "arch_contains",
    description = """
    Check if an architecture contains an entity.
        Arguments:
            arch_longname: The long name of the architecture.
            ent_id: The entity ID.
            recursive: Optional boolean to search child architectures as well (default False).
        Returns:
            True if the entity is contained in the architecture, False otherwise.
    """)

def arch_contains(arch_longname: str, ent_id: int, recursive: Optional[bool] = None) -> bool:
    """Check if an architecture contains an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    if recursive is None:
        return arch.contains(ent)
    else:
        return arch.contains(ent, recursive)

@mcp.tool(
    name = "arch_ents",
    description = """
    Get entities within an architecture.
        Arguments:
            arch_longname: The long name of the architecture.
            recursive: Optional boolean to consider child architectures (default False).
        Returns:
            A list of Entity objects.
    """)

def arch_ents(arch_longname: str, recursive: Optional[bool] = None) -> List[dict]:
    """Get entities within an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    if recursive is None:
        entities = arch.ents()
    else:
        entities = arch.ents(recursive)
    return [asdict(ent_to_entity(ent)) for ent in entities]

@mcp.tool(
    name = "arch_metric",
    description = """
    Get metric value(s) for an architecture.
        Arguments:
            arch_longname: The long name of the architecture.
            metric: A metric name, or a list of metric names.
            metric_format: Optional format string ("auto", "raw", or "string", default "auto").
        Returns:
            A dictionary of metric values if metric is a list, or a single metric value if metric is a string.
    """)

def arch_metric(arch_longname: str, metric: Union[str, List[str]], metric_format: Optional[str] = None):
    """Get metric value(s) for an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    if metric_format is None:
        return arch.metric(metric)
    else:
        return arch.metric(metric, metric_format)

@mcp.tool(
    name = "arch_metrics",
    description = """
    Get metric names defined for an architecture.
        Arguments:
            arch_longname: The long name of the architecture.
        Returns:
            A list of Metric objects.
    """)

def arch_metrics(arch_longname: str) -> List[dict]:
    """Get metric names for an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    metric_ids = arch.metrics()
    return [asdict(metric_id_to_metric(metric_id)) for metric_id in metric_ids]

@mcp.tool(
    name = "arch_name",
    description = """
    Get the short name of an architecture.
        Arguments:
            arch_longname: The long name of the architecture.
        Returns:
            The short name of the architecture as a string.
    """)

def arch_name(arch_longname: str) -> str:
    """Get the short name of an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    return arch.name()

@mcp.tool(
    name = "arch_parent",
    description = """
    Get the parent of an architecture.
        Arguments:
            arch_longname: The long name of the architecture.
        Returns:
            The parent architecture longname as a string, or None if it is a root architecture.
    """)

def arch_parent(arch_longname: str) -> Optional[str]:
    """Get the parent of an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    parent = arch.parent()
    if parent is None:
        return None
    return parent.longname()

@mcp.tool(
    name = "arch_depends",
    description = """
    Get dependencies of an architecture.
        Arguments:
            arch_longname: The long name of the architecture.
            recursive: Optional boolean to include child architecture dependencies (default True).
            group: Optional boolean to group the returned keys into as few keys as possible (default False).
        Returns:
            A list of ArchDependency objects representing architectures this architecture depends on.
    """)

def arch_depends(arch_longname: str, recursive: Optional[bool] = None, group: Optional[bool] = None) -> List[dict]:
    """Get dependencies of an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    
    # Call depends() with appropriate arguments based on optional parameters
    # Both parameters are optional with defaults (recursive=True, group=False)
    # If only group is provided, we need to pass recursive with its default (True) first
    if recursive is not None and group is not None:
        dependencies_dict = arch.depends(recursive=recursive, group=group)
    elif recursive is not None:
        dependencies_dict = arch.depends(recursive=recursive)
    elif group is not None:
        # Pass recursive=True (default) first, then group
        dependencies_dict = arch.depends(recursive=True, group=group)
    else:
        dependencies_dict = arch.depends()
    
    dependencies = []
    for dep_arch, refs in dependencies_dict.items():
        ref_list = [ref_to_ref(ref) for ref in refs]
        dependencies.append(ArchDependency(arch_longname=dep_arch.longname(), refs=ref_list))
    return [asdict(dep) for dep in dependencies]

@mcp.tool(
    name = "arch_dependsby",
    description = """
    Get architectures that depend on this architecture.
        Arguments:
            arch_longname: The long name of the architecture.
            recursive: Optional boolean to include child architecture dependencies (default True).
            group: Optional boolean to group the returned keys into as few keys as possible (default False).
        Returns:
            A list of ArchDependency objects representing architectures that depend on this architecture.
    """)

def arch_dependsby(arch_longname: str, recursive: Optional[bool] = None, group: Optional[bool] = None) -> List[dict]:
    """Get architectures that depend on this architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    
    # Call dependsby() with appropriate arguments based on optional parameters
    # Both parameters are optional with defaults (recursive=True, group=False)
    # If only group is provided, we need to pass recursive with its default (True) first
    if recursive is not None and group is not None:
        dependencies_dict = arch.dependsby(recursive=recursive, group=group)
    elif recursive is not None:
        dependencies_dict = arch.dependsby(recursive=recursive)
    elif group is not None:
        # Pass recursive=True (default) first, then group
        dependencies_dict = arch.dependsby(recursive=True, group=group)
    else:
        dependencies_dict = arch.dependsby()
    
    dependencies = []
    for dep_arch, refs in dependencies_dict.items():
        ref_list = [ref_to_ref(ref) for ref in refs]
        dependencies.append(ArchDependency(arch_longname=dep_arch.longname(), refs=ref_list))
    return [asdict(dep) for dep in dependencies]

@mcp.tool(
    name = "lexer_lexeme",
    description = """
    Get the lexeme at a specific line and column for an entity.
        Arguments:
            ent_id: The entity ID (must be a file entity).
            line: The line number (1-based).
            column: The column number (1-based).
        Returns:
            A Lexeme object at the specified location.
    """)

def lexer_lexeme(ent_id: int, line: int, column: int) -> dict:
    """Get the lexeme at a specific line and column."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    try:
        lexer = ent.lexer()
    except understand.UnderstandError:
        raise ValueError(f"Entity with id {ent_id} does not have a lexer")
    lexeme = lexer.lexeme(line, column)
    if lexeme is None:
        raise ValueError(f"No lexeme found at line {line}, column {column}")
    return asdict(lexeme_to_lexeme(lexeme))

@mcp.tool(
    name = "lexer_lexemes",
    description = """
    Get a list of lexemes for an entity.
        Arguments:
            ent_id: The entity ID (must be a file entity).
            start_line: Optional start line number (1-based).
            end_line: Optional end line number (1-based).
        Returns:
            A list of Lexeme objects.
    """)

def lexer_lexemes(ent_id: int, start_line: Optional[int] = None, end_line: Optional[int] = None) -> List[dict]:
    """Get a list of lexemes for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    try:
        lexer = ent.lexer()
    except understand.UnderstandError:
        raise ValueError(f"Entity with id {ent_id} does not have a lexer")
    
    # Call lexemes() with appropriate arguments based on optional parameters
    if start_line is not None and end_line is not None:
        lexemes = lexer.lexemes(start_line, end_line)
    elif start_line is not None:
        lexemes = lexer.lexemes(start_line)
    else:
        lexemes = lexer.lexemes()
    
    return [asdict(lexeme_to_lexeme(lex)) for lex in lexemes]

@mcp.tool(
    name = "lexer_lines",
    description = """
    Get the number of lines in the lexer for an entity.
        Arguments:
            ent_id: The entity ID (must be a file entity).
        Returns:
            The number of lines as an integer.
    """)

def lexer_lines(ent_id: int) -> int:
    """Get the number of lines in the lexer."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    try:
        lexer = ent.lexer()
    except understand.UnderstandError:
        raise ValueError(f"Entity with id {ent_id} does not have a lexer")
    return lexer.lines()

@mcp.tool(
    name = "ent_control_flow_graph",
    description = """
    Get the control flow graph for an entity.
        Arguments:
            ent_id: The entity ID (must be a function or method entity).
        Returns:
            A list of CFNode objects representing the control flow graph nodes.
    """)

def ent_control_flow_graph(ent_id: int) -> List[dict]:
    """Get the control flow graph for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    cfg = ent.control_flow_graph()
    if cfg is None:
        raise ValueError(f"Entity with id {ent_id} does not have a control flow graph")
    nodes = cfg.nodes()
    # Convert each node to CFNode dataclass, passing the node list for ID lookup
    return [asdict(cfnode_to_cfnode(node, nodes)) for node in nodes]

@mcp.tool(
    name = "kind_list_entity",
    description = """
    Get a list of entity kinds that match a filter.
        Arguments:
            entkind: Optional entity kind filter string. If not provided, all entity kinds are returned.
        Returns:
            A list of kind longnames (strings) that match the filter.
    """)

def kind_list_entity(entkind: Optional[str] = None) -> List[str]:
    """Get a list of entity kinds that match a filter."""
    if entkind is None:
        kinds = understand.Kind.list_entity()
    else:
        kinds = understand.Kind.list_entity(entkind)
    return [kind.longname() for kind in kinds]

@mcp.tool(
    name = "kind_list_reference",
    description = """
    Get a list of reference kinds that match a filter.
        Arguments:
            refkind: Optional reference kind filter string. If not provided, all reference kinds are returned.
        Returns:
            A list of kind longnames (strings) that match the filter.
    """)

def kind_list_reference(refkind: Optional[str] = None) -> List[str]:
    """Get a list of reference kinds that match a filter."""
    if refkind is None:
        kinds = understand.Kind.list_reference()
    else:
        kinds = understand.Kind.list_reference(refkind)
    return [kind.longname() for kind in kinds]

@mcp.tool(
    name = "ent_refs",
    description = """
    Get references for an entity.
        Arguments:
            ent_id: The entity ID.
            refkindstring: Optional reference kind filter string.
            entkindstring: Optional entity kind filter string.
            unique: Optional boolean to return only the first matching reference to each unique entity (default False).
        Returns:
            A list of Ref objects.
    """)

def ent_refs(ent_id: int, refkindstring: Optional[str] = None, entkindstring: Optional[str] = None, unique: Optional[bool] = None) -> List[dict]:
    """Get references for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    
    if refkindstring is None:
        refs = ent.refs()
    elif entkindstring is None:
        refs = ent.refs(refkindstring)
    elif unique is None:
        refs = ent.refs(refkindstring, entkindstring)
    else:
        refs = ent.refs(refkindstring, entkindstring, unique)
    
    return [asdict(ref_to_ref(ref)) for ref in refs]

@mcp.tool(
    name = "ent_ref",
    description = """
    Get the first reference for an entity (same as refs()[:1]).
        Arguments:
            ent_id: The entity ID.
            refkindstring: Optional reference kind filter string.
            entkindstring: Optional entity kind filter string.
        Returns:
            A Ref object, or None if no reference is found.
    """)

def ent_ref(ent_id: int, refkindstring: Optional[str] = None, entkindstring: Optional[str] = None) -> Optional[dict]:
    """Get the first reference for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    
    if refkindstring is None:
        ref = ent.ref()
    elif entkindstring is None:
        ref = ent.ref(refkindstring)
    else:
        ref = ent.ref(refkindstring, entkindstring)
    
    if ref is None:
        return None
    return asdict(ref_to_ref(ref))

@mcp.tool(
    name = "ent_filerefs",
    description = """
    Get file references for a file entity.
        Arguments:
            ent_id: The entity ID (must be a file entity).
            refkindstring: Optional reference kind filter string.
            entkindstring: Optional entity kind filter string.
            unique: Optional boolean to return only the first matching reference to each unique entity (default False).
        Returns:
            A list of Ref objects. Returns empty list for non-file entities.
    """)

def ent_filerefs(ent_id: int, refkindstring: Optional[str] = None, entkindstring: Optional[str] = None, unique: Optional[bool] = None) -> List[dict]:
    """Get file references for a file entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    
    if refkindstring is None:
        refs = ent.filerefs()
    elif entkindstring is None:
        refs = ent.filerefs(refkindstring)
    elif unique is None:
        refs = ent.filerefs(refkindstring, entkindstring)
    else:
        refs = ent.filerefs(refkindstring, entkindstring, unique)
    
    return [asdict(ref_to_ref(ref)) for ref in refs]


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