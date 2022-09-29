# Draw a UML Sequence Diagram for each method

import understand
import sys
#import os
#sys.setdlopenflags(sys.getdlopenflags() | os.RTLD_GLOBAL)


def sortedEntities(db):
  for ent in sorted(db.ents("method ~unknown ~unresolved"),key= lambda ent: ent.name()):
    imgFile = ent.name()+".png"
    print (ent.name() + "["+ ent.kindname() + "]" + "->" + imgFile)
    ent.draw("UML Sequence Diagram",imgFile)


if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  sortedEntities(db)
