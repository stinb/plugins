# Understand API - www.scitools.com
# Generate edge and node files for import into Gephi, the open source graphing tool

import understand
import sys
import csv
from collections import defaultdict

metricList=["maxcyclomatic","sumcyclomatic","countline","countlinecode"]
archs=[]
ents=[]
entArchs=defaultdict(lambda: defaultdict(lambda: None))

def prepEntArchs():
  for root in archs:
    findArchs(root,root)

def findArchs(root,arch):
  for ent in arch.ents():
    pretty_arch=arch.longname().split("/")[1] #Only show the top level architecture to reduce total groups
    entArchs[ent.id()][root.name()]=pretty_arch
  for child in arch.children():
    findArchs(root,child)

#Create a csv file with all of the Node information
def nodeFile():
  filecsv= open("nodes.csv", "w")
  csvwriter = csv.writer(filecsv,dialect='excel',quoting=csv.QUOTE_MINIMAL)
  header = ["Id","Label"]
  header = header+metricList+archs
  csvwriter.writerow(header)
  for file in ents:
    row = [file.id(),file.relname()]
    metrics= file.metric(metricList)
    for metric in metricList:
      row.append(metrics[metric])
    for arch in archs:
      thisarch=entArchs[file.id()][arch.name()]
      row.append(thisarch)
    csvwriter.writerow(row)
  filecsv.close()
  
#Create a CSV file with the edge information
def edgeFile():
  filecsv=open("edges.csv","w")
  csvwriter = csv.writer(filecsv,dialect='excel',quoting=csv.QUOTE_MINIMAL)
  header = ["Source","Target","Weight"]
  csvwriter.writerow(header)
  for file in ents:
    depList=file.depends()
    for ent in depList: 
      if ent in ents:  #Don't show non project dependencies
        row =file.id(), ent.id(), len(depList[ent])
        csvwriter.writerow(row)
  filecsv.close()

if __name__ == '__main__':
  args = sys.argv
  db = understand.open(args[1])
  archs = db.root_archs()
  ents = db.ents("File ~unknown ~unresolved")

  prepEntArchs()
  nodeFile()
  edgeFile()