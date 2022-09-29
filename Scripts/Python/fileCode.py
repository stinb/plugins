#Walk through a specified file and print the code from that file without the comments

import understand
import sys


# Open Database
args = sys.argv;
db = understand.open(args[1]);

def fileCleanText(file):
  returnString = "";
  
  # Open the file lexer with macros expanded and 
  # inactive code removed
  for lexeme in file.lexer(False,8,False,True):
    if(lexeme.token() != "Comment"):
      # Go through all lexemes in the file and append 
      # the text of non-comments to returnText   
      returnString += lexeme.text();
  return returnString;

# Search for the first file named 'test' and print 
# the file name and the cleaned text
file = db.lookup(".*test.*","file")[0];
print (file.longname());
print(fileCleanText(file)); 
  
  
