import understand

def metricsTable(report, metriclist, targetlist, link=False, filenameformat="long", names=dict(), prog_min = 0, prog_max=100):
  tbldesc = """ [{ "name" : "Name", "filtertype": "string", "sort" : "ascending" } """
  for metric in metriclist:
    tbldesc += ",{ \"name\": \"" + names.get(metric,metric) + "\", \"filtertype\": \"numeric\"}"
  tbldesc += "]"
  report.table(tbldesc)

  prog_range = len(targetlist)
  last_prog = prog_min
  for i, target in enumerate(targetlist):
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
    if prog_min is not None:
      cur_prog = int(prog_min + (i / prog_range * (prog_max-prog_min))) # percentage
      if cur_prog > last_prog:
        report.progress(cur_prog)
        last_prog = cur_prog
    if report.is_aborted():
      return
  report.table()

def targetMetricsTable(report, target, metrics, names=dict()):
  report.table("""
    [{
      "name" : "Metric",
      "filtertype": "string",
      "sort" : "ascending"
     },{
      "name" : "Value",
      "filtertype": "numeric"
    }]""")
  for k,v in target.metric(metrics).items():
    report.tablecell()
    report.print(names.get(k, k))

    report.tablecell()
    report.print(str(v))
  report.table()
