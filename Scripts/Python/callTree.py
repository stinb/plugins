"""
:Title: Textual Call Trees
:Description: Print an alphabetically sorted list of functions and their call trees
:Arg: Takes an Understand database folder (.und) as the first argument
:Example: upython callTree.py myproject.und
:Tags: python, script, call tree, calls

This script is designed for use with Understand (https://www.scitools.com)
For updated scripts, documentation and license info see the Understand Plugins repo
     https://github.com/stinb/plugins
"""

# 
import understand
import sys

def printCallTree(ent,depth,seen):
  print(("|"*depth),ent.name())
  if (ent.id() in seen):
    return
  seen[ent.id()]=1
  for ref in sorted(ent.refs("call","",True),key= lambda ref: ref.ent().name()):
    printCallTree(ref.ent(),depth+1,seen);

	
if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  for ent in sorted(db.ents("function ~unknown ~unresolved"),key= lambda ent: ent.name()):
    seen = {}
    printCallTree(ent,0,seen)
    print ("\n\n")
  

  


