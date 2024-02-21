# This script generates design structure matrices (DSM) and related metrics
# based on the research paper "Hidden Structure: Using Network Methods to Map
# Product Architecture" https://dash.harvard.edu/handle/1/10646422
# It's meant to be imported

import understand
from statistics import median
from functools import cmp_to_key

class DepMatrix:
  class FileData:
    def __init__(self, file, allowed):
      self.file = file
      self.direct = set(file.depends().keys()).intersection(allowed)
      self.direct.add(file) # Always assume self dependency
      self.visible = set()
      self.directFanIn = None
      self.visibleFanIn = None
      self.category = None

    def name(self,format):
      if format == "long":
        return self.file.longname()
      elif format == "relative":
        return self.file.relname()
      else:
        return self.file.name()

    def value(self, metric, defaultValue=None):
      if metric == "dfi" and self.directFanIn is not None:
        return self.directFanIn
      elif metric == "dfo":
        return len(self.direct)
      elif metric == "vfi" and self.visibleFanIn is not None:
        return self.visibleFanIn + self.directFanIn
      elif metric == "vfo":
        return len(self.direct) + len(self.visible)
      elif metric == "class" and self.category is not None:
        return self.category
      return defaultValue

  # File matrix
  def __init__(self, entities=set(), progress=None):
    self.graph = dict()
    self.order = []
    self.metrics = []
    self.type = "No Classification"
    self.categories = []
    self.values = dict() # Metrics for the overall matrix
    self.values["size"] = len(entities)

    if entities:
      self.metrics.append("dfo")
      progressMax = self.size()
      for i, file in enumerate(entities):
        self.order.append(file)
        self.graph[file] = self.FileData(file, entities)
        if progress and i % 20 == 0:
          progress("\rCalculating Dependencies: %d of %d entities" % (i+1, progressMax))
      if progress:
        progress("\rCalculating Dependencies: %d of %d entities\n" % (progressMax, progressMax))
      self.calcVisibility(progress)
      self.calcFanIns()

  def calcVisibility(self, progress=None):
    if "vfo" in self.metrics:
      return
    self.metrics.append("vfo")
    total = 0
    dirtotal = 0
    progressMax = self.size()
    for i, data in enumerate(self.graph.values()):
      visited = set()
      toVisit = list(data.direct)
      while toVisit:
        file = toVisit.pop() # Take from back
        if not file in visited:
          visited.add(file)
          toVisit.extend(list(self.graph[file].direct)) # add to back
      data.visible = visited.difference(data.direct)
      total += data.value("vfo")
      dirtotal += data.value("dfo")
      if progress and i % 20 == 0:
        progress("\rCalculating Visibility: %d of %d entities" % (i+1, progressMax))
    if progress:
      progress("\rCalculating Visibility: %d of %d entities\n" % (progressMax, progressMax))
    if self.size():
      self.values["propagation_cost"] = total / (self.size() * self.size()) * 100 # percentage
      self.values["dir_propagation_cost"] = dirtotal / (self.size() * self.size()) * 100 # percentage

  def calcFanIns(self):
    if "vfi" in self.metrics or ("vfo" not in self.metrics and "dfo" in self.metrics):
      return
    vfi = 0 if "vfo" in self.metrics else None
    for data in self.graph.values():
      data.directFanIn = 0
      data.visibleFanIn = vfi
    for data in self.graph.values():
      for file in data.direct:
        self.graph[file].directFanIn += 1
      for file in data.visible:
        self.graph[file].visibleFanIn += 1
    if "dfi" not in self.metrics:
      self.metrics.append("dfi")
    if "vfo" in self.metrics and "vfi" not in self.metrics:
      self.metrics.append("vfi")

  def calcCycles(self):
    """
    Find the size, vfi, and vfo of the largest cycle and the size of the next
    largest cycle and store them in the values dictionary of the matrix
    """
    if "core_size" in self.values:
      return


    # Tarjan's algorithm for finding strongly connected components
    # https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm
    index = 0
    indices = dict()
    lowlink = dict()
    stack = list()
    components = list()

    def connectRecursive(file):
      nonlocal index
      indices[file] = index
      lowlink[file] = index
      index += 1
      stack.append(file)

      for toFile in self.graph[file].direct:
        if toFile not in indices:
          connectRecursive(toFile)
          lowlink[file] = lowlink[file] if lowlink[file] < lowlink[toFile] else lowlink[toFile]
        elif toFile in stack:
          lowlink[file] = lowlink[file] if lowlink[file] < lowlink[toFile] else lowlink[toFile]

      if lowlink[file] == indices[file]:
        idx = stack.index(file)
        components.append(stack[idx:])
        del stack[idx:]

    for file in self.order:
      if file not in indices:
        connectRecursive(file)

    components.sort(key=len)
    self.values["core_size"] = len(components[-1])
    if self.size() > 0:
      self.values["core_percent"] = len(components[-1]) / self.size() * 100
    self.values["second_largest_cycle_size"] = 0 if len(components) < 2 else len(components[-2])
    for file in components[-1]:
      self.values["core_vfi"] = self.graph[file].value("vfi")
      self.values["core_vfo"] = self.graph[file].value("vfo")
      break
    return components

  def calcMedians(self, metrics):
    found = 0
    lists = dict()
    for m in metrics:
      lists[m] = []
      if "median_" + m in self.values:
        found += 1
    if found == len(metrics):
      return # already calculated

    for data in self.graph.values():
      for m in metrics:
        lists[m].append(data.value(m))
    for m in metrics:
      self.values["median_"+m] = median(lists[m])

  def findType(self):
    cnt = self.size()
    if cnt < 1:
      return

    self.calcCycles()
    largest = self.values["core_size"]
    second = self.values["second_largest_cycle_size"]

    self.type = "Core-Periphery"
    if largest/cnt < 0.04:
      self.type = "Hierarchical"
    elif largest < second * 1.5:
      self.type = "Multi-Core"
    elif largest/cnt <= 0.06:
      self.type = "Borderline Core-Periphery"

  def classify(self, strategy="auto"):
    # Allow calling this function with different strategies, so clear
    # previous results
    for prevCategory in self.categories:
      if prevCategory in self.values:
        del self.values[prevCategory]

    self.findType()
    if strategy == "median" or (strategy == "auto" and self.type in ["Hierarchical", "Multi-Core"]):
      # hierarchical or multi-core, use medians
      self.calcMedians(["vfi","vfo"])
      vfo_pivot = self.values["median_vfo"]
      vfi_pivot = self.values["median_vfi"]
      self.categories = ["Shared-M", "Central", "Periphery-M", "Control-M"]
    else:
      vfo_pivot = self.values["core_vfo"]
      vfi_pivot = self.values["core_vfi"]
      self.categories = ["Shared", "Core", "Periphery", "Control"]

    SHARED=0
    CORE=1
    PERIPHERAL=2
    CONTROL=3
    cnts = [0,0,0,0]
    for data in self.graph.values():
      type = CORE
      if data.value("vfi") >= vfi_pivot and data.value("vfo") < vfo_pivot:
        type =SHARED
      elif data.value("vfi") < vfi_pivot and data.value("vfo") < vfo_pivot:
        type = PERIPHERAL
      elif data.value("vfi") < vfi_pivot and data.value("vfo") >= vfo_pivot:
        type = CONTROL

      data.category = type
      cnts[type] += 1

    if not "class" in self.metrics:
      self.metrics.append("class")
    for i, category in enumerate(self.categories):
      self.values[category] = cnts[i]

  def nameAt(self,idx, format):
    return self.graph[self.order[idx]].name(format)

  def valueAt(self,idx, metric):
    return self.graph[self.order[idx]].value(metric)

  def entityAt(self, idx):
    return self.order[idx]

  def entityValue(self, ent, metric):
    return self.graph[ent].value(metric)

  def matrixValue(self,row, column, visibility=True):
    return (self.order[column] in self.graph[self.order[row]].direct or
            (visibility and self.order[column] in self.graph[self.order[row]].visible))

  def orderBy(self,fields, directions, nameformat="long"):
    def compare(file1, file2):
      for i, field in enumerate(fields):
        diff = 0
        if field == "filename":
          key1 = self.graph[file1].name(nameformat)
          key2 = self.graph[file2].name(nameformat)
          if key1 < key2:
            diff = -1
          elif key2 < key1:
            diff = 1
        else:
          diff = self.graph[file1].value(field,0) - self.graph[file2].value(field,0)
        if diff != 0:
          if i < len(directions) and directions[i] != "ascending":
            return -diff
          return diff
      return diff
    if fields:
      self.order.sort(key=cmp_to_key(compare))

  def size(self):
    return self.values["size"]

  def printMatrix(self,out, nameformat, visibility=True):
    # Header
    for i in range(self.size()):
      print("," + self.nameAt(i,nameformat), end="",file=out)
    print(file=out)

    # Print the matrix
    for i in range(self.size()):
      print (self.nameAt(i,nameformat), end="", file=out)
      for j in range(self.size()):
        print (",", end="",file=out)
        if self.matrixValue(i,j, visibility):
          print("1", end="",file=out)
      print(file=out)

  def printMetrics(self,out, nameformat):
    cols = sorted(self.metrics)
    print ("files," + ",".join(cols),file=out)
    for i in range(self.size()):
      print(self.nameAt(i, nameformat),end="",file=out)
      for col in cols:
        print("," + str(self.valueAt(i,col)), end="", file=out)
      print(file=out)

