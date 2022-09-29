# Print a sorted list of functions and their call trees
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
  

  


