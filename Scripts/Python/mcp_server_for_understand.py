import argparse
import os
import sys
from dataclasses import dataclass, asdict
from typing import Annotated, List, Literal, Optional, Union
from pydantic import Field

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

@mcp.tool(name="understand_version")
def understand_version() -> str:
    """Return the Understand API version."""
    return understand.version()

@mcp.tool(name="lookup")
def lookup(
    name: Annotated[str, Field(description="The name or regex pattern to search for.")],
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string (e.g., 'Function', 'File').")] = None,
) -> List[dict]:
    """Look up entities by name in the Understand database."""
    if db is None:
        raise RuntimeError("Database is not open")
    
    entities = db.lookup(name, kindstring)
    return [asdict(ent_to_entity(ent)) for ent in entities]

@mcp.tool(name="lookup_uniquename")
def lookup_uniquename(
    uniquename: Annotated[str, Field(description="The unique name of the entity.")],
) -> Optional[dict]:
    """Look up an entity by its unique name."""
    if db is None:
        raise RuntimeError("Database is not open")
    
    ent = db.lookup_uniquename(uniquename)
    if ent is None:
        return None
    return asdict(ent_to_entity(ent))

@mcp.tool(name="ents")
def ents(
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string (e.g., 'Function', 'File', 'Class').")] = None,
) -> List[dict]:
    """Get entities from the database, optionally filtered by kind."""
    if db is None:
        raise RuntimeError("Database is not open")
    
    entities = db.ents(kindstring)
    return [asdict(ent_to_entity(ent)) for ent in entities]

@mcp.tool(name="db_ent_from_id")
def db_ent_from_id(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> Optional[dict]:
    """Get an entity by its ID."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        return None
    return asdict(ent_to_entity(ent))

@mcp.tool(name="db_files")
def db_files() -> List[dict]:
    """Get all file entities in the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    files = db.files()
    return [asdict(ent_to_entity(ent)) for ent in files]

@mcp.tool(name="db_language")
def db_language() -> List[str]:
    """Get the project languages."""
    if db is None:
        raise RuntimeError("Database is not open")
    return list(db.language())

@mcp.tool(name="db_metric")
def db_metric(
    metric: Annotated[Union[str, List[str]], Field(description="A metric name, or a list of metric names.")],
    metric_format: Annotated[Literal["auto", "raw", "string"], Field(description="Output format.")] = "auto",
):
    """Get metric value(s) for the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    return db.metric(metric, metric_format)

@mcp.tool(name="db_name")
def db_name() -> str:
    """Get the filename of the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    return db.name()

@mcp.tool(name="ent_comments")
def ent_comments(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    style: Annotated[Literal["before", "after"], Field(description="Comment location.")] = "before",
    raw: Annotated[bool, Field(description="Preserve original formatting.")] = False,
    refkindstring: Annotated[Optional[str], Field(description="Optional reference kind string.")] = None,
):
    """Get comments associated with an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.comments(style, raw, refkindstring)

@mcp.tool(name="ent_contents")
def ent_contents(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> str:
    """Get the contents of an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.contents()

@mcp.tool(name="ent_ents")
def ent_ents(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    refkindstring: Annotated[Optional[str], Field(description="Optional reference kind filter string.")] = None,
    entkindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string.")] = None,
) -> List[dict]:
    """Get entities that reference or are referenced by an entity."""
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

@mcp.tool(name="ent_file_type")
def ent_file_type(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> str:
    """Get file type for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.file_type()

@mcp.tool(name="ent_freetext")
def ent_freetext(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    option: Annotated[str, Field(description="Type of information (e.g., 'AllowExceptions', 'Inline').")],
):
    """Get freetext for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.freetext(option)

@mcp.tool(name="ent_ib")
def ent_ib(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    options: Annotated[Optional[str], Field(description="Formatting options.")] = None,
) -> List[str]:
    """Get Info Browser information for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.ib(options)

@mcp.tool(name="ent_kindname")
def ent_kindname(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> str:
    """Get kindname for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.kindname()

@mcp.tool(name="ent_language")
def ent_language(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> str:
    """Get language for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.language()

@mcp.tool(name="ent_library")
def ent_library(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> str:
    """Get library for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.library()

@mcp.tool(name="ent_longname")
def ent_longname(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> str:
    """Get longname for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.longname()

@mcp.tool(name="ent_name")
def ent_name(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    template_args: Annotated[bool, Field(description="Include template instantiation arguments.")] = False,
) -> str:
    """Get name for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.name(template_args)

@mcp.tool(name="ent_parameters")
def ent_parameters(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    shownames: Annotated[bool, Field(description="Include parameter names.")] = True,
) -> Optional[str]:
    """Get parameters for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.parameters(shownames)

@mcp.tool(name="ent_parent")
def ent_parent(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> Optional[dict]:
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

@mcp.tool(name="ent_parsetime")
def ent_parsetime(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> int:
    """Get parsetime for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.parsetime()

@mcp.tool(name="ent_relname")
def ent_relname(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> Optional[str]:
    """Get relname for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.relname()

@mcp.tool(name="ent_simplename")
def ent_simplename(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> str:
    """Get the simple name of an entity (deprecated)."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.simplename()

@mcp.tool(name="ent_type")
def ent_type(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> str:
    """Get type for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.type()

@mcp.tool(name="ent_uniquename")
def ent_uniquename(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> str:
    """Get uniquename for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.uniquename()

@mcp.tool(name="ent_value")
def ent_value(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> str:
    """Get value for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.value()

@mcp.tool(name="ent_violations")
def ent_violations(
    ent_id: Annotated[int, Field(description="The entity ID (must be a file entity).", ge=1)],
) -> Optional[List[dict]]:
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

@mcp.tool(name="db_violations")
def db_violations() -> List[dict]:
    """Get all violations in the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    violations = db.violations()
    return [asdict(violation_to_violation(viol)) for viol in violations]

@mcp.tool(name="ent_annotations")
def ent_annotations(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> List[dict]:
    """Get annotations for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    annotations = ent.annotations()
    return [asdict(atn_to_atn(atn)) for atn in annotations]

@mcp.tool(name="db_annotations")
def db_annotations() -> List[dict]:
    """Get all annotations in the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    annotations = db.annotations()
    return [asdict(atn_to_atn(atn)) for atn in annotations]

@mcp.tool(name="metric_list")
def metric_list(
    kindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string.")] = None,
    filter_enabled: Annotated[bool, Field(description="Filter to only enabled metrics.")] = True,
) -> List[dict]:
    """List available metrics."""
    if db is None:
        raise RuntimeError("Database is not open")
    
    # Handle different parameter combinations for understand.Metric.list
    if kindstring is None:
        metric_ids = understand.Metric.list(db=db, filter=filter_enabled)
    else:
        metric_ids = understand.Metric.list(kindstring, db=db, filter=filter_enabled)
    
    return [asdict(metric_id_to_metric(metric_id)) for metric_id in metric_ids]

@mcp.tool(name="db_metrics")
def db_metrics() -> List[dict]:
    """Get project level metrics from the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    metric_ids = db.metrics()
    return [asdict(metric_id_to_metric(metric_id)) for metric_id in metric_ids]

@mcp.tool(name="ent_metrics")
def ent_metrics(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
) -> List[dict]:
    """Get metrics for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    metric_ids = ent.metrics()
    return [asdict(metric_id_to_metric(metric_id)) for metric_id in metric_ids]

@mcp.tool(name="ent_metric")
def ent_metric(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    metric: Annotated[Union[str, List[str]], Field(description="A metric name, or a list of metric names.")],
    metric_format: Annotated[Literal["auto", "raw", "string"], Field(description="Output format.")] = "auto",
):
    """Get metric value(s) for an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.metric(metric, metric_format)

@mcp.tool(name="ent_kind_check")
def ent_kind_check(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    kindstring: Annotated[str, Field(description="A kind filter string to match against.")],
) -> bool:
    """Check if an entity's kind matches a filter string."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    return ent.kind().check(kindstring)

@mcp.tool(name="ent_depends")
def ent_depends(
    ent_id: Annotated[int, Field(description="The entity ID (must be a class or file entity).", ge=1)],
) -> List[dict]:
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

@mcp.tool(name="ent_dependsby")
def ent_dependsby(
    ent_id: Annotated[int, Field(description="The entity ID (must be a class or file entity).", ge=1)],
) -> List[dict]:
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

@mcp.tool(name="db_root_archs")
def db_root_archs() -> List[str]:
    """Get root architectures for the database."""
    if db is None:
        raise RuntimeError("Database is not open")
    archs = db.root_archs()
    return [arch.longname() for arch in archs]

@mcp.tool(name="db_archs")
def db_archs(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    implicit: Annotated[bool, Field(description="Return architectures that contain any of the entity's parents.")] = False,
) -> List[str]:
    """Get architectures that contain an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
    archs = db.archs(ent, implicit)
    return [arch.longname() for arch in archs]

@mcp.tool(name="arch_children")
def arch_children(
    arch_longname: Annotated[str, Field(description="The long name of the architecture.")],
) -> List[str]:
    """Get children of an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    children = arch.children()
    return [child.longname() for child in children]

@mcp.tool(name="arch_contains")
def arch_contains(
    arch_longname: Annotated[str, Field(description="The long name of the architecture.")],
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    recursive: Annotated[bool, Field(description="Search child architectures as well.")] = False,
) -> bool:
    """Check if an architecture contains an entity."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    ent = db.ent_from_id(ent_id)
    if ent is None:
        raise ValueError(f"Entity with id {ent_id} not found")
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
    entities = arch.ents(recursive)
    return [asdict(ent_to_entity(ent)) for ent in entities]

@mcp.tool(name="arch_metric")
def arch_metric(
    arch_longname: Annotated[str, Field(description="The long name of the architecture.")],
    metric: Annotated[Union[str, List[str]], Field(description="A metric name, or a list of metric names.")],
    metric_format: Annotated[Literal["auto", "raw", "string"], Field(description="Output format.")] = "auto",
):
    """Get metric value(s) for an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    return arch.metric(metric, metric_format)

@mcp.tool(name="arch_metrics")
def arch_metrics(
    arch_longname: Annotated[str, Field(description="The long name of the architecture.")],
) -> List[dict]:
    """Get metric names for an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    metric_ids = arch.metrics()
    return [asdict(metric_id_to_metric(metric_id)) for metric_id in metric_ids]

@mcp.tool(name="arch_name")
def arch_name(
    arch_longname: Annotated[str, Field(description="The long name of the architecture.")],
) -> str:
    """Get the short name of an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    return arch.name()

@mcp.tool(name="arch_parent")
def arch_parent(
    arch_longname: Annotated[str, Field(description="The long name of the architecture.")],
) -> Optional[str]:
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

@mcp.tool(name="arch_depends")
def arch_depends(
    arch_longname: Annotated[str, Field(description="The long name of the architecture.")],
    recursive: Annotated[bool, Field(description="Include child architecture dependencies.")] = True,
    group: Annotated[bool, Field(description="Group the returned keys.")] = False,
) -> List[dict]:
    """Get dependencies of an architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    
    dependencies_dict = arch.depends(recursive=recursive, group=group)
    dependencies = []
    for dep_arch, refs in dependencies_dict.items():
        ref_list = [ref_to_ref(ref) for ref in refs]
        dependencies.append(ArchDependency(arch_longname=dep_arch.longname(), refs=ref_list))
    return [asdict(dep) for dep in dependencies]

@mcp.tool(name="arch_dependsby")
def arch_dependsby(
    arch_longname: Annotated[str, Field(description="The long name of the architecture.")],
    recursive: Annotated[bool, Field(description="Include child architecture dependencies.")] = True,
    group: Annotated[bool, Field(description="Group the returned keys.")] = False,
) -> List[dict]:
    """Get architectures that depend on this architecture."""
    if db is None:
        raise RuntimeError("Database is not open")
    arch = db.lookup_arch(arch_longname)
    if arch is None:
        raise ValueError(f"Architecture with longname '{arch_longname}' not found")
    
    dependencies_dict = arch.dependsby(recursive=recursive, group=group)
    dependencies = []
    for dep_arch, refs in dependencies_dict.items():
        ref_list = [ref_to_ref(ref) for ref in refs]
        dependencies.append(ArchDependency(arch_longname=dep_arch.longname(), refs=ref_list))
    return [asdict(dep) for dep in dependencies]

@mcp.tool(name="lexer_lexeme")
def lexer_lexeme(
    ent_id: Annotated[int, Field(description="The entity ID (must be a file entity).", ge=1)],
    line: Annotated[int, Field(description="The line number (1-based).", ge=1)],
    column: Annotated[int, Field(description="The column number (1-based).", ge=1)],
) -> dict:
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

@mcp.tool(name="lexer_lexemes")
def lexer_lexemes(
    ent_id: Annotated[int, Field(description="The entity ID (must be a file entity).", ge=1)],
    start_line: Annotated[Optional[int], Field(description="Optional start line number (1-based).", ge=1)] = None,
    end_line: Annotated[Optional[int], Field(description="Optional end line number (1-based).", ge=1)] = None,
) -> List[dict]:
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

@mcp.tool(name="lexer_lines")
def lexer_lines(
    ent_id: Annotated[int, Field(description="The entity ID (must be a file entity).", ge=1)],
) -> int:
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

@mcp.tool(name="ent_control_flow_graph")
def ent_control_flow_graph(
    ent_id: Annotated[int, Field(description="The entity ID (must be a function or method entity).", ge=1)],
) -> List[dict]:
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

@mcp.tool(name="kind_list_entity")
def kind_list_entity(
    entkind: Annotated[Optional[str], Field(description="Optional entity kind filter string.")] = None,
) -> List[str]:
    """Get a list of entity kinds that match a filter."""
    if entkind is None:
        kinds = understand.Kind.list_entity()
    else:
        kinds = understand.Kind.list_entity(entkind)
    return [kind.longname() for kind in kinds]

@mcp.tool(name="kind_list_reference")
def kind_list_reference(
    refkind: Annotated[Optional[str], Field(description="Optional reference kind filter string.")] = None,
) -> List[str]:
    """Get a list of reference kinds that match a filter."""
    if refkind is None:
        kinds = understand.Kind.list_reference()
    else:
        kinds = understand.Kind.list_reference(refkind)
    return [kind.longname() for kind in kinds]

@mcp.tool(name="ent_refs")
def ent_refs(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    refkindstring: Annotated[Optional[str], Field(description="Optional reference kind filter string.")] = None,
    entkindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string.")] = None,
    unique: Annotated[bool, Field(description="Return only the first matching reference to each unique entity.")] = False,
) -> List[dict]:
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
    else:
        refs = ent.refs(refkindstring, entkindstring, unique)
    
    return [asdict(ref_to_ref(ref)) for ref in refs]

@mcp.tool(name="ent_ref")
def ent_ref(
    ent_id: Annotated[int, Field(description="The entity ID.", ge=1)],
    refkindstring: Annotated[Optional[str], Field(description="Optional reference kind filter string.")] = None,
    entkindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string.")] = None,
) -> Optional[dict]:
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

@mcp.tool(name="ent_filerefs")
def ent_filerefs(
    ent_id: Annotated[int, Field(description="The entity ID (must be a file entity).", ge=1)],
    refkindstring: Annotated[Optional[str], Field(description="Optional reference kind filter string.")] = None,
    entkindstring: Annotated[Optional[str], Field(description="Optional entity kind filter string.")] = None,
    unique: Annotated[bool, Field(description="Return only the first matching reference to each unique entity.")] = False,
) -> List[dict]:
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
    elif unique is False: # default
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