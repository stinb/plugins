#Print all of the comments in each file matching a specified search string

import understand
import sys


# Open Database
args = sys.argv;
db = understand.open(args[1]);

def fileComment(file):
  return file.comments("after");

# Search for the first file named 'test' and print 
# the file name and the cleaned text
file = db.lookup(".*test.*","file")[0];
print (file.longname());
print(fileComment(file)); 
  
  
