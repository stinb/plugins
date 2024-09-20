# Compliance Report
import datetime
import pathlib
import re

import understand

# https://stackoverflow.com/questions/4836710/is-there-a-built-in-function-for-string-natural-sort
def natural_sort(l):
    convert = lambda text: int(text) if text.isdigit() else text.lower()
    alphanum_key = lambda key: [convert(c) for c in re.split('([0-9]+)', key)]
    return sorted(l, key=alphanum_key)

def printheading(report, inspection, name, configOnly = False):
  report.heading(1)
  report.print("{} {}".format(inspection.config().name(), name))

  report.align("center")
  if not configOnly:
    report.heading(6)
    report.print("for the")

    report.heading(1)
    report.print("{} Project".format(pathlib.Path(report.db().name()).stem))

  report.heading(2)
  report.print("Understand Version: {}".format(understand.version()))

  report.heading(3)
  report.print(datetime.datetime.now().strftime("%c"))
  report.heading()
  report.align()

def printSummaryRow(report, fields):
  report.align("center")
  for name,value in fields:
    report.bold()
    report.print(" {}: ".format(name))
    report.nobold()
    report.print(str(value))
  report.print("\n")
  report.align()
