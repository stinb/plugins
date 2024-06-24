#Walk through a document lexically.

import understand
import sys

def run(db,searchstring):
  file = db.lookup(searchstring,"file")[0]
  print (file)
  for lexeme in file.lexer():
    print (lexeme.text(),end="")
    if lexeme.ent():
      print ("@",end="")
  

if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])

  searchstring = ".Web.";
  run(db,searchstring)     