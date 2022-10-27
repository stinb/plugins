# Demonstration of the add_to_arch method
# Blog Article: https://blog.scitools.com/automating-a-custom-architecture/
# This demonstration assigns functions to an architecture based on
# the possible depth of their call tree.

import understand
import sys

def depth(ent):
  level = -1
  cur = [ent]
  next = []
  visited = set()
  while len(cur):
    level += 1
    for curEnt in cur:
      if curEnt in visited:
        continue
      visited.add(curEnt)
      for ref in curEnt.refs("call"):
        next.append(ref.ent())
    cur = next
    next = []
  return level

db = understand.open(sys.argv[1]);
cache = dict()
for ent in db.ents("function"):
  ent.add_to_arch("calllevels/"+str(depth(ent)))


