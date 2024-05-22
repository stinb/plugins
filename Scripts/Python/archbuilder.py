"""
:Title: Architecture based on call depth
:Description: This sample script creates a new architecture and assigns functions to 
              sub-architectures based on the maximum depth of their call tree.
              For example a function with a max call depth of 4 levels will be added
              to an architecture named "4"
:Link: https://blog.scitools.com/automating-a-custom-architecture/
:Arg: Takes an Understand database folder (.und) as the first argument
:Example: upython archbuilder.py myproject.und
:Tags: python, script, template, architectures, calls

This script is designed for use with Understand (https://www.scitools.com)
For updated scripts, documentation and license info see the Understand Plugins repo
     https://github.com/stinb/plugins
"""
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


