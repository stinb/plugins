"""Reading the project's annotation-template definitions.

The definitions live in the project directory's annotation_templates.json.
Reports read that file directly: the Python API deliberately keeps its
surface small -- an annotation is consumed through atn.text() and parsed
with deviations.fields_from_note -- but a coverage report legitimately
needs the definitions themselves (labels for empty columns, which fields
are required), and the project file is their source of truth.

Personal (per-user) templates are not in the project file; annotations
using one fall back to the fields observed in the data, the same way a
deleted template's do."""

import json
import os


def load(db):
  """The project's template definitions: a list of dicts with id, name,
  body, offerOnIgnore, and fields (each with key, label, type, required).
  A missing or unreadable file reads as no templates."""
  path = os.path.join(db.name(), "annotation_templates.json")
  try:
    with open(path, encoding="utf-8") as f:
      data = json.load(f)
  except (OSError, ValueError):
    return []

  result = []
  for tmpl in data.get("templates", []):
    fields = []
    for field in tmpl.get("fields", []):
      label = field.get("label", "")
      fields.append({
        "key": field.get("key", label) or label,
        "label": label,
        "type": field.get("type", "text"),
        "required": bool(field.get("required", False)),
      })
    result.append({
      "id": tmpl.get("id", ""),
      "name": tmpl.get("name", ""),
      "body": tmpl.get("body", ""),
      "offerOnIgnore": bool(tmpl.get("offerOnIgnore", False)),
      "fields": fields,
    })
  return result
