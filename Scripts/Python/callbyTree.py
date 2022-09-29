# Print a sorted list of entities and their call-by trees
import understand
import sys

def printCallByTree(ent,depth,seen):
  print(("|"*depth),ent.name())
  if (ent.id() in seen):
    return
  seen[ent.id()]=1
  for ref in sorted(ent.refs("callby","",True),key= lambda ref: ref.ent().name()):
    printCallByTree(ref.ent(),depth+1,seen);

	
if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  for ent in sorted(db.ents("function ~unknown ~unresolved"),key= lambda ent: ent.name()):
    seen = {}
    printCallByTree(ent,0,seen)
    print ("\n\n")
  

  


