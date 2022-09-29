# Print a sorted list of functions and their Information Browsers
import understand
import sys


	
if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  for ent in sorted(db.ents("function ~unknown ~unresolved"),key= lambda ent: ent.name()):
    list = ent.ib("{calledby}levels=-1;{calledby}sort=None");
    for line in list:
      print(line.rstrip('\n'))
  

  


