# The bar layout this graph plugin uses requires build 1191 or later

import bisect
import math
import locale
locale.setlocale(locale.LC_ALL, '')

BINS=10
AUTO_LOG_RANGE=25

def define_histogram_opts(graph):
  """ Define the following options:

  - RankDir
  - Node Labels
  - Log Scale

  - Threshold
  - Cutoff
  """
  graph.options().define("RankDir", ["LR", "RL", "TB", "BT"], "BT")
  graph.options().define("Node Labels", ["Off", "On"], "On")
  graph.options().define("Log Scale", ["Off", "On", "Auto"], "Auto")

  graph.options().define("Threshold", ["None", "Max Integer", "Max Percent", "Min Integer", "Min Percent"], "None")

  # Messy, since we really just want to pass in an integer, but support integers
  # between 0 and 100
  cutoffs = []
  for i in range(0,101):
    cutoffs.append(str(i))
  graph.options().define("Cutoff", cutoffs, "0")

def draw_entity_histogram(graph, ents, metric, tooltip_kindstr="", bin_count=BINS):
  graph.set("layout", "bar") # Override default layout algorithm (Usually dot)
  graph.set("rankdir", graph.options().lookup("RankDir"))
  graph.default("color", "#4e8cb9", "node")
  graph.default("fillcolor", "#4e8cb9", "node")
  graph.default("fontcolor", "#FFFFFF", "node")

  vals = []
  minimum = None
  maximum = None
  isfloat = False
  for ent in ents:
    v = ent.metric(metric)
    if v is not None:
      if isinstance(v,str):
        if v.endswith('%'):
          v = v[:-1]
        v = float(v)
        isfloat = True
      vals.append(v)
      if minimum is None or v < minimum:
        minimum = v
      if maximum is None or v > maximum:
        maximum = v
    if graph.is_aborted():
      return

  if not vals:
    return # nothing to draw

  binwidth = (maximum - minimum) / bin_count
  if not isfloat:
    binwidth = max(int(binwidth + 0.5), 1)
  bins = [minimum]
  if binwidth > 0:
    while bins[-1] <= maximum:
      bins.append(minimum + binwidth*len(bins))
  else:
    bins.append(minimum + 1)

  cnts = [0] * len(bins)
  for v in vals:
    idx = bisect.bisect_left(bins, v)
    if idx > 0 and bins[idx] != v:
      idx -= 1
    cnts[idx] += 1

  logopt = graph.options().lookup("Log Scale")
  if logopt == "Auto":
    # use log scale if range is greater than AUTO_LOG_RANGE. Skip last
    # when calculating min because last should always be 0
    logscale = (max(cnts) - min(cnts[:-1])) > AUTO_LOG_RANGE
  else:
    logscale = logopt == "On"

  # Create stacked bar chart from counts
  node_labels = graph.options().lookup("Node Labels") == "On"
  for i in range(len(bins)-1):
    c = graph.cluster(" {} ".format(format_number(bins[i], isfloat)))
    if not cnts[i]:
      continue

    area = cnts[i]
    n = c.node(format_number(cnts[i], False)) if node_labels else c.node()
    n.set("tooltip", "{}{}with {} value >= {} and < {}".format(format_number(area,False),tooltip_kindstr,metric,format_number(bins[i],isfloat,True),format_number(bins[i+1],isfloat,True)))

    if logscale:
      area = math.log10(area) + 1 # make sure area is greater than 0
    n.set("area", str(area))

    if fails_threshold(graph, bins[i]):
      n.set("color", "#e45721")
      n.set("fillcolor", "#e45721")

def fails_threshold(graph, value):
  threshold = graph.options().lookup("Threshold")
  if threshold == "None":
    return False # No thresholding, does not fail

  cutoff = int(graph.options().lookup("Cutoff"))
  if threshold.endswith("Percent"):
    cutoff = cutoff / 100.0
  if threshold.startswith("Max"):
    return value > cutoff
  return value < cutoff

def format_number(val, isfloat, tooltip=False):
  if isfloat:
    return "{:.2f}".format(val)
  elif len(str(val)) > 5 and not tooltip:
    return "{:g}".format(val)
  return "{:n}".format(val)
