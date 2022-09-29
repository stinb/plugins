# Print a sorted list of root entities and their call trees
# At each level print the entities defined in that function

import understand
import sys

def printCallTree(ent,depth,seen):
  print(("|"*depth),ent.name())
  for ref in sorted(ent.refs("define","",True),key= lambda ref: ref.ent().name()):
    print(("|"*depth),"   ",ref.ent().type(),ref.ent().name())
  if (ent.id() in seen):
    return
  seen[ent.id()]=1
  for ref in sorted(ent.refs("call","",True),key= lambda ref: ref.ent().name()):
    printCallTree(ref.ent(),depth+1,seen);
    
	
if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  for ent in sorted(db.ents("function ~unknown ~unresolved, method ~unknown ~unresolved"),key= lambda ent: ent.name()):
    
    # Skip to next function if not a root entity
    if (ent.refs("callby","",True)):
      continue
    
    # Skip to next function if it is a system library function
    if (ent.library()):
      continue
      
    seen = {}
    printCallTree(ent,0,seen)
    print ("\n\n")
  

  


