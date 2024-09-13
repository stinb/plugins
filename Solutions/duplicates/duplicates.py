import understand

class Location:
  '''File and line'''
  def __init__(self, file, line):
    self.file = file
    self.line = line

  def __eq__(self, other):
    return isinstance(other, self.__class__) and self.file == other.file and self.line == other.line

  def __ne__(self, other):
      return not self.__eq__(other)


def shifted(locations, amount):
  '''Shift location lines by amount'''
  locs = []
  for loc in locations:
    locs.append(Location(loc.file, loc.line + amount))
  return locs

class Match:
  '''Track the start locations and length of a duplication'''
  def __init__(self, locations, length):
    self.locations = sorted(locations, key = lambda loc: (loc.file, loc.line))
    self.length = length

  def grow(self, amount = 1):
    self.length += amount

  def weight(self):
    return len(self.locations)

def fileLines(file, ignoredTokens = ["Whitespace", "Comment", "Newline"]):
  '''Return a list of lines to be considered and their corresponding original
     line numbers, skipping tokens in ignoredTokens'''
  try:
    lexer = file.lexer(False)
  except understand.UnderstandError:
    return [[],[]]

  lineDict = dict()
  for lexeme in lexer:
    if lexeme.token() in ignoredTokens:
      continue
    if lexeme.line_begin() not in lineDict:
      lineDict[lexeme.line_begin()] = ""
    lineDict[lexeme.line_begin()] += lexeme.text()

  text = []
  numbers = []
  for linenum, linetext in sorted(lineDict.items()):
    if linetext: # ignore blank lines
      text.append(linetext)
      numbers.append(linenum)

  return text, numbers

def addToPluginCache(duplLines, plugin):
  '''Store the duplicated line counts in the cache'''
  if not hasattr(plugin, "cache"):
    return
  cache = plugin.cache("Duplicates")
  if not cache:
    return
  cache.clear()
  total = 0
  for file, lineset in duplLines.items():
    cache.insert(len(lineset), ent=file)
    total += len(lineset)
  cache.insert(total)


class Matches:
  '''Store a graph created from source code and find matches from the graph'''

  class Node:
    '''Internal, graph node'''
    def __init__(self):
      self.children = dict()

    def add(self, id, location):
      self.children.setdefault(id,[]).append(location)

    def weight(self):
      return len(self.locations())

    def branches(self):
      return list(self.children.keys())

    def locations(self):
      locs = []
      for loclist in self.children.values():
        locs += loclist
      return locs

  def __init__(self, lines):
    self.nodes = dict()
    self.nodes[None] = self.Node()
    self.lines = lines

  def processFile(self, file, lines):
    '''Add a file to the graph. lines is a list of strings containing the
       processed file contents. '''
    ids = []
    for i in range(len(lines) - (self.lines - 1)):
      ids.append("".join(lines[i:i+self.lines]))
    ids.append(None)
    for i in range(len(ids) - 1):
      self.nodes.setdefault(ids[i],self.Node()).add(ids[i+1], Location(file, i))

  def duplicateLines(self):
    '''Return a dictionary from file to set of duplicated lines in that file'''
    dupl = dict()
    for node in self.nodes.values():
      if node.weight() > 1:
        for loc in node.locations():
          for i in range(self.lines):
            dupl.setdefault(loc.file, set()).add(loc.line + i)
    return dupl

  def findMatches(self):
    '''Return a list of Match objects'''
    visited = set()
    matchDict = dict()
    branchMatch = dict()
    for id, node in self.nodes.items():
      # If weight is 1, not duplicated, so not a match
      w = node.weight()
      if id in visited or w < 2:
        continue
      visited.add(id)

      # Create a match for the duplication
      match = Match(node.locations(), self.lines)

      # Keep track of potential overlaps to process in a second pass
      branches = node.branches()
      nextId = id

      # Grow this match
      while len(branches) == 1 and self.nodes[branches[0]].weight() == w:
        nextId = branches[0]
        if nextId in visited: # child was processed, merge and delete child
          prev = matchDict[nextId]
          match.grow(prev[0].length - self.lines + 1)
          nextId = prev[1]
          del matchDict[nextId]
          matchDict[id] = (match, nextId)
          branches = None
          break
        else: # first visit to child, mark child as processed
          match.grow()
          if nextId is not None:
            visited.add(nextId)
        branches = self.nodes[nextId].branches()

      if branches is None:
        continue # this node joined with an already built node

      # Create matches for branches
      hadBranch = False
      for childId, locs in self.nodes[nextId].children.items():
        if len(locs) > 1:
          hadBranch = True
          if len(locs) == w:
            nextId = childId
            break
          shift = match.length - self.lines
          branchMatch.setdefault(nextId,[]).append((Match(locs, self.lines), childId))

      if not hadBranch or nextId == id:
        nextId = None

      matchDict[id] = (match, nextId)


    # All matches have been made, combine matchDict into branchMatch dictionary
    for k,v in matchDict.items():
      branchMatch.setdefault(k, []).append(v)


    # Second pass merging to grow branch matches. Loop over list of keys
    # because branchMatch is updated by the loop
    keys = list(branchMatch.keys())
    for k1 in keys:
      i1 = 0
      maxI = len(branchMatch.get(k1, []))
      while i1 < maxI: # for each match starting with key
        i2 = 0
        m1, k2 = branchMatch[k1][i1]
        maxI2 = len(branchMatch.get(k2, []))
        while i2 < maxI2: # for each next possible match
          m2, k3 = branchMatch[k2][i2]
          if m2.locations == shifted(m1.locations,1): # merge?
            break
          i2 += 1
        if i2 < maxI2: # merge
          m1.grow(m2.length - self.lines + 1) # merge match 2 into match 1
          branchMatch[k1][i1] = (m1, k3)      # update match 1 in dictionary
          del branchMatch[k2][i2]             # remove match 2 in dictionary

          # Stay at current index to see if this match can continue growing.
          # Just in case, check if removed item was from current list and update
          # indices accordingly
          if k1 == k2:
            maxI -= 1
            if i1 > i2:
              i1 -= 1
        else:
          # This match cannot grow, move on to the next one
          i1 += 1

    # Keep only the matches and return them
    matchlist = []
    for k, v1 in branchMatch.items():
      for v in v1:
        m = v[0]
        # an unmerged but continuing branch is one longer, so grow
        if v[1] is not None:
          m.grow(1)
        matchlist.append(m)

    return matchlist

if __name__ == "__main__":
  import understand
  db = understand.open("/Users/natasha/projects/Sample/setrefgraph/setrefgraph.und")

  matches = Matches(5)
  for file in db.ents("file ~unknown ~unresolved"):
    matches.processFile(file, fileLines(file)[0])

  matchlist = matches.findMatches()
  lineNumbers = dict()
  for m in sorted(matchlist, key = lambda m: (m.weight() * m.length, m.length), reverse=True):
    print ("Found", m.length, "duplicated lines in", m.weight(), "locations:")
    for loc in m.locations:
      if loc.file not in lineNumbers:
        lineNumbers[loc.file] = fileLines(loc.file)[1]
      print ("    ", loc.file.longname(), lineNumbers[loc.file][loc.line], "-", lineNumbers[loc.file][loc.line + m.length - 1])
