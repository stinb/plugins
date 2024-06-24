#Find all 'File' entities that match a search string

import understand
import sys

def searchFileString(db,searchstring):
  for file in (db.lookup(searchstring,"File")):
    #If the file is from the Ada Standard library, skip to the next
    if file.library() != "Standard":
      print (file.relname())
  

if __name__ == '__main__':
  searchstring = ".Test."
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  searchFileString(db,searchstring)      