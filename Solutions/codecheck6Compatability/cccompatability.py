# Compatability Common


def init(report):
  report.options().label("error", "")
  report.options().checkbox("showignored",  "Show Ignored", False)
  report.options().checkbox("showexcluded",  "Show Excluded", False)
  report.options().checkbox("ignoredonlyresults",  "Ignored Only Results", False)
  report.options().checkbox("showbaselineignored",  "Show Baseline Ignored", False)
  report.options().checkbox("showbaselineignored",  "Show Baseline Ignored", False)

def test_options(report):
  report.options().label("error", "")

  showIgnored = report.options().lookup("showignored")
  onlyFPResults = report.options().lookup("ignoredonlyresults")
  if showIgnored and onlyFPResults:
    report.options().label("error", "Error: cannot check \"Show Ignored\" and \"Ignored Only Results\". Only \"Show Ignored\" will apply.")
  # No critical errors with und
  return True

def hasFP(report):
  return report.options().lookup("showignored") or report.options().lookup("ignoredonlyresults")


def violations(report, inspection):
  showIgnored = report.options().lookup("showignored")
  showExcluded = report.options().lookup("showexcluded")
  showBaseline = report.options().lookup("showbaselineignored")
  onlyFPResults = report.options().lookup("ignoredonlyresults")

  files = inspection.files()
  vlist = []
  if showIgnored:
    if showBaseline:
      vlist = inspection.violations()
    else:
      for v in inspection.violations():
        isBaseline = False
        isExcluded = False
        for source,note in v.ignores():
          if source == "Excluded":
            isExcluded = True
          if source == "Baseline":
            isBaseline = True
        if not isBaseline:
          if isExcluded:
            if showExcluded and v.file() in files:
              vlist.append(v)
          else:
            vlist.append(v)
  else:
    for v in inspection.violations():
      if bool(v.ignores()) == onlyFPResults:
        isExcluded = False
        for source,note in v.ignores():
          if source == "Excluded":
            isExcluded = True
        if isExcluded:
          if showExcluded and v.file() in files:
            vlist.append(v)
        else:
          vlist.append(v)

  return vlist

def checkinfo(inspection):
  checkname = dict()
  checkseverity = dict()
  conf = inspection.config()
  if conf:
    for check_id in conf.checks():
      checkname[check_id] = conf.check_name(check_id)
      severity = ""
      for tag in conf.check_tags(check_id):
        if tag.startswith("Severity: "):
          severity = tag[10:]
      checkseverity[check_id] = severity
  return checkname, checkseverity

# [name, children, vlist]
def addToTree(treeDict, pathlist, violation):
  par = ""
  if par not in treeDict:
    treeDict[par] = [par, [], []]

  for i in range(len(pathlist)):
    key = '\n'.join(pathlist[0:i+1])
    if not key in treeDict:
      treeDict[key] = [pathlist[i], [], []]
      treeDict[par][1].append(key)
    par = key

  treeDict[par][2].append(violation)

def printTreeDict(report, treeDict, cols, checkname, checkseverity, cnt):
  report.table(cols)

  report.tablecell()
  report.print("Number of Results: " + str(cnt))
  for i in range(len(cols)-1):
    report.tablecell() # Skip to end of row

  if not treeDict:
    return

  toVisit = [""]
  while toVisit:
    curlist = treeDict[toVisit.pop()]
    toVisit.extend(sorted(curlist[1],reverse=True))

    if curlist[0]:
      report.tablecell()
      report.print(curlist[0])

      for i in range(len(cols)-1):
        report.tablecell() # Skip to end of row

    for v in curlist[2]:
      violationTableRow(report, v, cols, checkname, checkseverity)


def violationTableRow(report, v, cols, checkname, checkseverity, filenames=None):
  ent = None
  if "Entity" in cols and v.ent_uniquename():
    ent = report.db().lookup_uniquename(v.ent_uniquename())

  sources = set()
  notes = []
  if "Note" in cols:
    for source, note in v.ignores():
      sources.add(source)
      if note:
        notes.append(note)

  for col in cols:
    report.tablecell()
    if col == "Results" or col == "Violation":
      report.print(v.text())

    # Location
    elif col == "File":
      report.print(filenames.get(v.file(), v.file()))
    elif col == "Line":
      report.print(str(v.line()))
    elif col == "Column":
      report.print(str(v.column()))

    # Entity
    elif col == "Entity" and ent:
      report.print(ent.longname())
    elif col == "Kind" and ent:
      report.print(ent.kind().name())

    # Check
    elif col == "CheckID":
      report.print(v.check_id())
    elif col == "Check Name":
      report.print(checkname.get(v.check_id(),""))
    elif col == "Check Short Description":
      pass # Deprecated attribute from before 7.0, usually empty
    elif col == "Severity":
      checkseverity.get(v.check_id(),"")

    # Ignores
    elif col == "Note":
      report.print('\n'.join(notes))
    elif col == "Ignored":
      if "Baseline" in sources:
        report.print("BaseLine Ignored")
      elif len(sources) == 1 and "Excluded" in sources:
        report.print("Excluded")
      elif sources:
        report.print("Ignored")
      elif v.ignores():
        report.print("true") # violations table

def fileNamesDict(report, format):
  filenames = dict()
  if format == "short":
    for file in report.db().files():
      filenames[file.longname()] = file.name()
  elif format == "relative":
    for file in report.db().files():
      filenames[file.longname()] = file.relname()
  return filenames
