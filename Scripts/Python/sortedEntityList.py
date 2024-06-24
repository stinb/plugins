# Print a sorted list of entities and their kind

import understand
import sys

def sortedEntities(db):
  for ent in sorted(db.ents(),key= lambda ent: ent.name()):
    print (ent.name(),"  [",ent.kindname(),"]",sep="",end="\n")
	
if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  sortedEntities(db)
  

  


