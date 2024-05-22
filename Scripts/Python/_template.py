"""
:Title: Understand Python API Sample Template
:Description: Opens a given understand project and lists the files in that project
:Arg: Takes an Understand database folder (.und) as the first argument
:Example: upython _template.py myproject.und
:Tags: python, script, template, files

This script is designed for use with Understand (https://www.scitools.com)
For updated scripts, documentation and license info see the Understand Plugins repo:
     https://github.com/stinb/plugins
"""
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
    