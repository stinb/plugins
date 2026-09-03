"""Reading the deviation record carried by a suppressed violation.

A violation can be suppressed by a comment in the code or by an annotation in
the project. Either way the suppression can carry a structured record -- the
fields of an annotation template offered in the Ignore Violation dialog -- and
this module turns those into one entry per suppression, plus the judgment of
whether that entry is a defensible deviation.

The report and the chart that visualizes it both read from here, so a
deviation is counted the same way wherever it is shown."""

import re

# A record line inside an ignore note: a short label, a colon, a value.
# Mirrors Ignores::parseNoteFields() in src/understand/codecheck/Ignores.cpp:
# same pattern, same 64-character label bound, same rule that the first
# non-matching line ends the record. The note is the single source of truth
# for a record -- however the ignore was stored, its note arrives whole,
# record lines and all -- and this module is the reader that parses it back.
# Change the two together.
FIELD_LINE = re.compile(r"^\s*([^:\n]{1,64}):\s*(.*)$")

# The fields an auditor looks for first, in the order they ask for them. Any
# other field a project records follows these, alphabetically.
CORE_FIELDS = ["Guideline", "Justification", "Rationale", "Risk Assessment",
               "Approved By", "Approval Date", "Severity", "Owner"]


def fields_from_note(note):
  """The record a whole ignore note carries, and the rationale left once the
  record lines are lifted off it for display.

  The note is kept whole at the source (parseNoteFields does not carve it up),
  so the split is done here, for the report only: the first line is the
  rationale, following "Label: value" lines are the record, and the first line
  that is not one ends the record and stays in the rationale. One exception:
  a record opens with its reserved "Template" line, and a suppression written
  with a template and no free-text rationale opens with the record itself --
  a first line naming "Template" IS the record, not a rationale. Returns the
  parsed record and that display rationale; the caller keeps the whole note if
  it needs it."""
  record = {}
  lines = note.split("\n")
  if not lines:
    return record, note.strip()

  start = 1
  first = FIELD_LINE.match(lines[0])
  if first and first.group(1).strip() == "Template":
    start = 0

  taken = start
  while taken < len(lines):
    match = FIELD_LINE.match(lines[taken])
    if not match:
      break
    record[match.group(1).strip()] = match.group(2).strip()
    taken += 1

  head = [] if start == 0 else [lines[0]]
  rationale = "\n".join(head + lines[taken:]).strip()
  return record, rationale


def deviations(inspection):
  """One entry per suppression: the violation, what silenced it, its
  rationale and its record. A violation silenced twice is two deviations,
  because each suppression is its own claim."""
  found = []
  for violation in inspection.violations():
    for source, note in violation.ignores():
      # The note arrives whole -- rationale and record lines together,
      # however the suppression was stored -- so the record is parsed
      # straight off it and a clean rationale lifted for the report.
      fields, rationale = fields_from_note(note or "")

      # "Template" is the record naming which template wrote it, not a field
      # anyone filled in: it reads as metadata, so it does not take a column
      # and does not count toward how documented a deviation is.
      template = fields.pop("Template", "")

      found.append({
        "violation": violation,
        "source": source or "Unknown",
        "template": template,
        # A rationale wrapped over several lines of a comment carries the
        # comment's indentation with it. It is read as one sentence, so show
        # it as one.
        "rationale": " ".join(rationale.split()),
        "fields": fields,
      })
  return found


def field_names(found):
  """Every field label the deviations carry, the ones an auditor reads first
  in front, the rest alphabetical."""
  seen = set()
  for entry in found:
    seen.update(entry["fields"].keys())
  ordered = [f for f in CORE_FIELDS if f in seen]
  ordered.extend(sorted(f for f in seen if f not in CORE_FIELDS))
  return ordered


def expected_fields(found):
  """What a complete deviation looks like in this project: the fields that
  most deviations already record. Measuring a gap against the team's own
  practice means the report does not fail a project for omitting a field this
  report invented, and does not pass one that quietly dropped a field it
  otherwise always records.

  Judged within each template: a Technical Debt deviation is not gappy for
  lacking the MISRA template's Guideline. Deviations that name no template
  form one group of their own. Returns {template: [labels]}."""
  groups = {}
  for entry in found:
    groups.setdefault(entry["template"], []).append(entry)

  expected = {}
  for template, entries in groups.items():
    counts = {}
    for entry in entries:
      for label in entry["fields"]:
        counts[label] = counts.get(label, 0) + 1
    threshold = max(2, len(entries) // 2)
    expected[template] = sorted(
      label for label, count in counts.items() if count >= threshold)
  return expected


def expected_of(entry, expected):
  """The field list this entry is judged against: its template's."""
  if isinstance(expected, dict):
    return expected.get(entry["template"], [])
  return expected  # a plain list judges everything alike


def gaps_of(entry, expected):
  """Why this deviation is not defensible yet, in an auditor's words. An
  empty list means it is."""
  reasons = []
  if not entry["rationale"] and not entry["fields"]:
    reasons.append("no justification recorded")
    return reasons
  # A free-text rationale is only expected of a freeform suppression. With a
  # template named, the record is the note -- the Ignore Violation dialog
  # offers no rationale box then, and the substance lives in the template's
  # own fields, judged below.
  if not entry["rationale"] and not entry["template"]:
    reasons.append("no rationale on the suppression itself")
  for label in expected_of(entry, expected):
    if not entry["fields"].get(label):
      reasons.append("missing " + label)
  return reasons


def split(found, expected):
  """The deviations that are fully documented and the ones that are not."""
  documented = []
  incomplete = []
  for entry in found:
    (incomplete if gaps_of(entry, expected) else documented).append(entry)
  return documented, incomplete
