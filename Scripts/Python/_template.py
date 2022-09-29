# Understand Python API Template
# Takes an Understand database file (.udb) as the first argument
# Then lists the files in that project

import understand
import sys

if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  #***************************Code Body***********************
  for file in db.ents("File"):
    print (file.name())  
    
  #***************************End Main Body ******************
    