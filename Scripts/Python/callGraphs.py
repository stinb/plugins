# Save a call graph image for each function

import understand
import sys
#import os


def sortedEntities(db):
  for ent in sorted(db.ents("function ~unknown ~unresolved"),key= lambda ent: ent.name()):
    imgFile = ent.name()+".png"
    print (ent.name() + "["+ ent.kindname() + "]" + "->" + imgFile)
    ent.draw("Calls",imgFile)


if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  sortedEntities(db)