"""
This is an approximation of the Information Browser, not a reproduction. It's
designed to allow grabbing the underlying data that is formatted into the IB
and uses configuration files that are NOT automatically updated and may
easily become out of date. The configuration files currently only cover C++.
"""

import inspect
import json
import re
import os

import understand

class IBFields:
  def __init__(self):
    # Load configuration files
    self.fields = dict()
    filename = os.path.join(os.path.dirname(inspect.getfile(lambda: None)), "ib_fields.json")
    with open(filename, 'r') as f:
      self.fields.update(json.loads(f.read()))

  def field_applies(self, ent, fieldname):
    if (not isinstance(ent, understand.Ent) or
        fieldname not in self.fields or  #unknown field name
        not fieldname.startswith(ent.language())): # wrong language
      return False

    field = self.fields[fieldname]
    if "ent_kind" in field and not ent.kind().check(field["ent_kind"]):
      return False

    if "ent_no_ref_kind" in field and ent.refs(field["ent_no_ref_kind"]):
      return False

    if (not field.get("destructors", True) and
        ent.kind().check("member function") and
        ent.name().startswith("~")):
      return False

    return True

  def field_value(self, ent, fieldname, db=None, opts = None):
    if not self.field_applies(ent, fieldname):
      return None

    field = self.fields[fieldname]
    options = field.get("options", {})
    if opts:
      options.update(opts)

    if field.get("expand_typedef", False):
      ent = expand_typedef(ent)
      if not ent:
        return None

    # References have first priority
    if "ref_kind" in field:
      rkind = str_from_maybe_dict(ent, field["ref_kind"])
      ent_kind_str = str_from_maybe_dict(ent, field.get("ref_ent_kind", ""))
      unique = field.get("ref_unique", False) or options.get("Reference", "Full") == "Simple"
      ref_func = understand.Ent.filerefs if field.get("file_refs",False) else understand.Ent.refs
      refs = ref_func(ent, rkind, ent_kind_str, unique)

      if "const_filter" in field:
        refs = const_filter(refs, field["const_filter"])

      if field.get("filter_external", False):
        refs = filter_external(refs)

      if refs: # fallbacks may exist so only use references if found
        if field.get("first_ref",False):
          if field.get("ref_field",None) == "file":
            return refs[0].file()
          return refs[0]
        return refs

    # Freetext
    if "freetext" in field:
      return ent.freetext(field["freetext"])

    # Metrics
    if "metrics" in field:
      return ent.metric(field["metrics"])

    # Arbitrary method like type() or library()
    if "method" in field:
      m = getattr(understand.Ent, field["method"])
      if m:
        return m(ent)

    # same name
    if field.get("same_name", False):
      return same_named(ent,db)

    return None # No value for field




def expand_typedef(ent):
  if not ent.kind().check("c typedef,c type alias"):
    return ent

  visited = set() # Avoid recursive typedefs
  while ent and not ent in visited and ent.kind().check("c typedef,c type alias"):
    visited.add(ent)
    refs = ent.refs("typed")
    ent = refs[0].ent() if refs else None

  return ent if not ent in visited else None

def const_filter(refs, is_const):
  keep = []
  const_re = re.compile(r"[ \*]?const($| )[^\*&]*$")
  for ref in refs:
    if bool(const_re.search(ref.ent().type())) == is_const:
      keep.append(ref)
  return keep

def definitionFile(ent):
  if ent.kind().check("file"):
    return ent

  refkind = "definein" if not ent.kind().check("static frominclude") else "~definein"
  refs = ent.refs(refkind)
  return refs[0].file() if refs else None

def filter_external(refs):
  keep = []
  deffile = dict()
  for ref in refs:
    if not ref.ent() in deffile:
      deffile[ref.ent()] = definitionFile(ref.ent())
    if ref.file() != deffile[ref.ent()]:
      keep.append(ref);
  return keep

def same_name(ent, db):
  if not db:
    return

  is_static = ent.kind().check("c static function ~member")
  is_template = ent.kind().check("c template")
  keep = []
  for overload in db.lookup(ent.longname(), "function"):
    if (overload == e or
        (is_static and overload.parent() != e.parent()) or
        (is_template and len(overload.refs("specialize")))):
      continue
    keep.append(overload)
  return keep

def str_from_maybe_dict(ent, val):
  def_val = val
  if isinstance(val, dict):
    for k,v in val.items():
      if not k:
        def_val = v
      elif ent.kind().check(k):
        return v
  return def_val






