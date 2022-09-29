# Create a CSV that shows each function call and location as well as where the called function is defined
import understand
import sys

def printCallPairs(ent):
  for ref in sorted(ent.refs("call","",True),key= lambda ref: ref.ent().name()):
    defineAref = ent.ref("definein")
    lineString = ent.longname()+"(\""+str(ent.parameters())+"\"),"
    lineString += defineAref.file().longname()+","
    lineString += str(ref.line())+","
    
    callee = ref.ent()
    defineBref = callee.ref("definein");
    lineString += callee.longname()+"(\""+str(callee.parameters())+"\"),"
    if defineBref is None:
      print(lineString)
      continue
    print (lineString+defineBref.file().longname())
    
    
if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  print("Caller, Caller File, Call Line, Callee, Callee File\n")
  for ent in sorted(db.ents("function ~unknown ~unresolved"),key= lambda ent: ent.name()):
    seen = {}
    printCallPairs(ent)
  

  


