# The display of the matrices uses matplotlib and numpy which are not part
# of the python version shipped with Understand. You can run the non-visual
# parts of the script with upython by using the switch --noshow to disable
# the visual display. Or, you can use your own version of python3 with
# matplotlib, numpy, and Understand to get the full features. See the following
# article on using Understand with custom Python installations.
# https://support.scitools.com/support/solutions/articles/70000582852-getting-started-with-the-python-api

import argparse
import os
import sys

import understand
from matrix import DepMatrix

parser = argparse.ArgumentParser(description="Calculate Dependency Matrices with Understand")
parser.add_argument("--matrixout", type=argparse.FileType('w'),nargs="?",const=sys.stdout,help='Print the matrix. If a file is given, print to the file, otherwise print to stdout')
parser.add_argument("--metricsout", type=argparse.FileType('w'),nargs="?",const=sys.stdout,help='Print the metrics. If a file is given, print to the file, otherwise print to stdout')
parser.add_argument("--direct",action="store_true", help="Use only direct dependencies instead of calculating the visibility matrix")
parser.add_argument("--noshow",action="store_true", help="Don't try to display the matrix with matplotlib")
parser.add_argument("--sort",choices=["filename","dfo","dfi","vfo","vfi","class"], action="append",help="Sort by field. Give the command multiple times to sort by first field then by second field")
parser.add_argument("--sortdir",default=[],choices=["ascending","descending"], action="append",help="The direction to sort the field, defaults to ascending. Specify multiple times to change order")
parser.add_argument("--nosort",action="store_true", help="If no --sort and --sortdir flags are present, don't sort. Otherwise, the default sort performed.")
parser.add_argument("--nameformat",default="short",choices=["short","long","relative"], help="File name format")
parser.add_argument("--strategy",default="auto",choices=["auto","median","core"], help="How to classify entries. Auto uses median for hierarchical or multi-core architectures")
parser.add_argument("--kinds",default="file ~unknown ~unresolved", help="A kindstring for the unit of interest. Dependencies can only be calculated for file or class entities.")
parser.add_argument("db", help="An Understand Database to retrieve dependencies from")

args = parser.parse_args()

db = understand.open(args.db)
ents = set(db.ents(args.kinds))
matrix = DepMatrix(ents,progress=sys.stdout.write)
if not matrix.size():
  print ("No Entities", file=sys.stderr)
  sys.exit()

matrix.classify(args.strategy)

if not args.nosort and not args.sort:
  args.sort = ["class","vfi","vfo","filename"]
  args.sortdir = ["ascending","descending"]

matrix.orderBy(args.sort, args.sortdir, args.nameformat)

if args.matrixout:
  matrix.printMatrix(args.matrixout, args.nameformat, not args.direct)

if args.metricsout:
  matrix.printMetrics(args.metricsout, args.nameformat)


print ("Database:", os.path.basename(db.name()))
print ("Architecture Type:", matrix.type)
for k,v in matrix.values.items():
  print(k + ":",v)

if not args.noshow:
  import matplotlib.pyplot as plt
  import matplotlib.patches as patches
  import numpy as np

  # Convert matrix to numpy matrix
  a = np.identity(matrix.size(), dtype='bool')
  for i in range(matrix.size()):
    for j in range(matrix.size()):
      a[i][j] = matrix.matrixValue(i,j, not args.direct)

  # Show the matrix
  fig = plt.figure()
  ax = fig.add_subplot(111)
  ax.set_title(os.path.basename(db.name()) + ": "+ matrix.type)
  ax.matshow(a, cmap="binary")
  ax.get_xaxis().set_visible(False)

  if matrix.categories and args.sort and args.sort[0] == "class":
    # Show the class rectangles
    last = 0
    ticks = []
    for cat in matrix.categories:
      cnt = matrix.values[cat]
      rect = patches.Rectangle((last, last), cnt, cnt, linewidth=1, edgecolor='b', facecolor='none')
      ticks.append(last + (cnt/2.0))
      ax.add_patch(rect)
      last += cnt
    ax.set_yticks(ticks, matrix.categories)
  else:
    ax.get_yaxis().set_visible(False)

  # Show file names on hover
  def format_coord(x, y):
    try:
        return "{} -> {}".format(matrix.nameAt(int(y),args.nameformat),matrix.nameAt(int(x),args.nameformat))
    except:
        return ""
  ax.format_coord = format_coord
  plt.show()

