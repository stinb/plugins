"""
:Title: Call Graph Images
:Description: Save a call graph image for each function in the project. The graphs will be 
              saved in the current working directory in the format function.png
              It also creates a simple index.html which shows all the graphs
:Arg: Takes an Understand database folder (.und) as the first argument
:Example: upython callGraphs.py myproject.und
:Tags: python, script, graphs, calls

This script is designed for use with Understand (https://www.scitools.com)
For updated scripts, documentation and license info see the Understand Plugins repo
     https://github.com/stinb/plugins
"""

# 

import understand
import sys

def sortedEntities(db):
  f = open("index.htm", "a")
  duplicateName = {}
  for ent in sorted(db.ents("function ~unknown ~unresolved"),key= lambda ent: ent.name()):
    count = duplicateName.setdefault(ent.name(), 0)
    dupCount = ""
    if (count):
      dupCount = duplicateName[ent.name()]
    imgFile = ent.name()+str(dupCount)+".png"
    print (ent.name() + " -> " + imgFile)
    ent.draw("Calls",imgFile)
    f.write("<a href=\""+imgFile+"\">"+ent.name()+"("+ent.parameters()+")</a><br>\n")
    duplicateName[ent.name()]+=1
  f.close()


if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  sortedEntities(db)