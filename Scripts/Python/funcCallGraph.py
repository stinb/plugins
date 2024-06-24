#Save Call Graphs of Function in Working Directory

import understand
import sys

OPTIONS = "name=shortname"
VARIANT = "Simplified"

def drawCallGraphs(db):
  func = db.ents("function ~unresolved,method ~unresolved,procedure ~unresolved")[0]
  file = "callsPY_" + func.name() + ".png"
  print (func.longname(),"->",file)
  try:
    func.draw("Calls",file,OPTIONS,VARIANT)
  except understand.UnderstandError as err:
    print("Error: {0}".format(err))
  

if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  drawCallGraphs(db)     
