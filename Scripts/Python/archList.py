"""
:Title: List Architectures
:Description: This sample script recursively lists all architectures in the project
:Arg: Takes an Understand database folder (.und) as the first argument
:Example: upython archList.py myproject.und
:Tags: python, script, template, architectures

This script is designed for use with Understand (https://www.scitools.com)
For updated scripts, documentation and license info see the Understand Plugins repo
     https://github.com/stinb/plugins
"""
import understand
import sys

def printArchs(arch,depth=0):
  string = ("| "* depth);
  string = string + arch.name()
  print (string)
  for ent in arch.ents(True):
    string = ("| "* depth)
    print(string,"*",ent)
  for child in arch.children():
    printArchs(child,depth+1)

  
if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  #***************************Code Body***********************
  for root in db.root_archs():
    printArchs(root)
    
  #***************************End Main Body ******************
    