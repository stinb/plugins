# List all local variables and where they are declared
# Organized by File and function/method


import understand
import sys

def fileList(db):
  for file in db.ents("File"):
    #If file is from the Ada Standard library, skip to next
    if file.library() != "Standard":
      print (file.relname())
      #List all functions defined in the file
      for functionRef in file.filerefs("define","function, method"):
        func = functionRef.ent()
        print ("    "+func.name())
        #list all varialbes defined in the function
        for variableRef in func.refs("define","object"):
          print ("        "+str(variableRef.line())+"  "+variableRef.ent().name())

if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  fileList(db)    
    