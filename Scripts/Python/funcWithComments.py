# Lists all functions, methods and procedures with associated comments

import understand
import sys

def sortKeyFunc(ent):
  return str.lower(ent.longname())

def run():
  ents = db.ents("""function ~unresolved ~unknown,
                    method ~unresolved ~unknown,
                    procedure ~unresolved ~unknown""")
  for ent in sorted(ents,key = sortKeyFunc):
    #If the file is from the Ada Standard library, skip to the next
    if ent.library() != "Standard":
      comments = ent.comments("after")
      if comments:
        print (ent.longname(),":\n  ",comments,"\n",sep="")
  

if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  run()      