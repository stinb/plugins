#Print the typedefs/enums and their associated enumerators

import understand
import sys

def findEnum(typeEnt):
  typeRef = typeEnt.ref("Typed")
  if typeRef is None:
    return
  enumEnt = typeRef.ent()
  for enumeratorRef in sorted(enumEnt.refs("define","Enumerator",True),key= lambda ref: ref.line()):
    enumerator = enumeratorRef.ent()
    print("\t"+enumerator.name()+" -> "+ enumerator.value())

	
if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  for ent in sorted(db.ents("typedef"),key= lambda ent: ent.name()):
    print (ent.name())
    findEnum(ent)
  

  


