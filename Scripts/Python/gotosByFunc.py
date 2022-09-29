# Walk through the lexemes for each function entity looking for keyword goto
# Takes an Understand database file (.udb) as the first argument

import understand
import sys
from datetime import datetime
from collections import defaultdict

lexerDict = defaultdict(int)

def returnLexemes(ent):
  definein = ent.ref("definein")
  if definein is None:
    return; #exit if there is no definition for this function
  file = definein.file()
  lineNum = definein.line()
  endLine = ent.ref("end").line()
  
  # Cache the lexers to provide a little more speed since creating
  # a new lexer is very expensive
  if lexerDict[file.id()] is 0:
    lexerDict[file.id()] = file.lexer(False,8,False,False)
    
  lexer = lexerDict[file.id()]
  lexeme = lexer.lexeme(lineNum, 0)
  while (lexeme and lexeme.line_end() <= endLine):
     if (lexeme.token() == 'Keyword' and lexeme.text() == 'goto'):
       print("Found goto in",ent.longname(),"(",ent.parameters(False),")"
       ,file.relname(),"(",lexeme.line_begin(),")");
     lexeme = lexeme.next()

if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  #***************************Code Body***********************
  startTime = datetime.now()
  ents = db.ents("function ~unknown ~unresolved")
  for ent in sorted(ents,key= lambda ent: ent.longname()):
    returnLexemes(ent)
  print("Script Runtime: ",datetime.now()-startTime)
    
  #***************************End Main Body ******************
    