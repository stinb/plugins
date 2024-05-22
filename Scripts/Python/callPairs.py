"""
:Title: Function Call Pairs
:Description: Create a CSV that shows each function call and location as well as where the called function is defined
              using the format: Caller, Caller File, Call Line, Callee, Callee File
              For example if funcA calls funcB on line 15 the output would be
              funcA(int arg1),c:/path/file1.c,15,funcB,c:/path/file2.c
:Link: https://blog.scitools.com/automating-a-custom-architecture/
:Arg: Takes an Understand database folder (.und) as the first argument
:Example: upython callPairs.py myproject.und
:Tags: python, script, calls, csv

This script is designed for use with Understand (https://www.scitools.com)
For updated scripts, documentation and license info see the Understand Plugins repo
     https://github.com/stinb/plugins
"""
# 
import understand
import sys

def printCallPairs(ent):
  for ref in sorted(ent.refs("call","",True),key= lambda ref: ref.ent().name()):
    defineAref = ent.ref("definein")
    lineString = ent.longname()+"(\""+str(ent.parameters())+"\"),"
    lineString += defineAref.file().longname()+","
    lineString += str(ref.line())+","
    
    callee = ref.ent()
    defineBref = callee.ref("definein");
    lineString += callee.longname()+"(\""+str(callee.parameters())+"\"),"
    if defineBref is None:
      print(lineString)
      continue
    print (lineString+defineBref.file().longname())
    
    
if __name__ == '__main__':
  # Open Database
  args = sys.argv
  db = understand.open(args[1])
  print("Caller, Caller File, Call Line, Callee, Callee File\n")
  for ent in sorted(db.ents("function ~unknown ~unresolved"),key= lambda ent: ent.name()):
    seen = {}
    printCallPairs(ent)
  

  


