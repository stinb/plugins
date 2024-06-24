#Lists all functions, methods and procedures with their parameters

import understand
import sys

def sortKeyFunc(ent):
  return str.lower(ent.longname())

def funcWithParameter(db):
  ents = db.ents("function,method,procedure")
  for func in sorted(ents,key = sortKeyFunc):
    #If the file is from the Ada Standard library, skip to the next
    if func.library() != "Standard":
      print (func.longname()," - ",func.parameters(),sep="",end="\n")
  

if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  funcWithParameter(db)      