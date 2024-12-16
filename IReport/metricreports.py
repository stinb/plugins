import understand

def metricsTable(report, metriclist, targetlist, link=False, filenameformat="long"):
  tbldesc = """ [{ "name" : "Name", "filtertype": "string", "sort" : "ascending" } """
  for metric in metriclist:
    tbldesc += ",{ \"name\": \"" + metric + "\", \"filtertype\": \"numeric\"}"
  tbldesc += "]"
  report.table(tbldesc)

  for target in targetlist:
    report.tablecell()
    if isinstance(target, understand.Arch):
      report.pagelink(target.longname())
      report.print(target.name())
    else:
      isfile = target.kind().check("file ~unresolved ~unknown")
      if isfile and link:
        report.pagelink(str(target.id()))
      elif not link:
        report.entity(target)
      if not isfile or filenameformat == "long":
        report.print(target.longname())
      elif filenameformat == "relative":
        report.print(target.relname())
      else:
        report.print(target.name())
    report.pagelink()
    report.entity()
    for m in metriclist:
      report.tablecell()
      val = target.metric(m)
      if val is not None:
        report.print(str(val))
  report.table()
