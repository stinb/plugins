import understand 
import glob
import os
import sys
import traceback
import re
# ##################################################################
# Please contact stephane.raynaud@Emenda.com for help/information
# This file is provided AS-IS
# ##################################################################
# Loader will create a data structure to explore the change in an entity
# If a comparison db is NOT set, it will use the same database for the comparison.
#
# To create the data structure, the algorithm will follow the following path
# It will PROGRESS from the current entity and seek all the reliationships of type "self.searchRelationship"
# to entity of type "self.searchKind"
# computeData() will STOP when MAX depth is reached (if MAX=-1, will not stop by depth) or
# if the entity has already been met and computed.
#
# When calling computeData, you can pass an options targets
# This is an array of "ids" (understand id) of possible entities you are looking
# to reach. If you pass targets, an extra information will be added to a link or node, i.e.
# "toTarget" will be set to true 
#
# The data structure is a TREE with the following element:
# AFTER_Entity,BEFORE_Entity,isComputed,isAdded,isRemoved,isSameContent,hasChangedInTree,ListOfLinks]
# AFTER_Entity: understand.Ent object : may be null
# BEFORE_Entity: understand.Ent object : may be null
# isComputed: True|False (was it computed or did we stop before)
# isAdded: True|False
# isRemoved: True|False
# isSameContent: True|False 
# hasChangedInTree: True|False
# ListOfLinks: A [,,,] list of Link
# toTarget: True|False (only valid if "targets" was passed to computeData)
#
# A Link, is 
# Reference Kind: String
# ListOfBEFORE_Refs: [,,,] of Understand.Ref
# ListOfAFTER_Refs: [,,,] of Understand.Ref
# isChanged: True|False (count of relationship has changed)
# isAdded: True|False
# isRemoved: True|False
# targetdata_BEFORE: Data for entity that this link is poiting to BEFORE (Can be None)
# targetdata_AFTER: Data for entity that this link is pointing to AFTER (Can be None)
# toTarget: True|False (only valid if "targets" was passed to computeData
#
# ##################################################################
class Loader:
  # ##################################################################
  def getFullDescriptionOfFunction(self,ent):
    result = ""
    if ent.type():
      result = ent.type() + " " + result
    if ent.longname():
      result = result + ent.longname()
    if ent.parameters():
      result = result + "(" + ent.parameters() + ")"
    Refs = ent.refs("DefineIn")
    if len(Refs)>=1:
      Ref = Refs[0]
      result = result + " [FILE:" + Ref.file().name()+"]"
    #result = result + " " + ent.uniquename()
    #If needed....
    return result
  # ##################################################################
  # definition of our unique key: Using plain-vanilla uniquename()
  def getUniqueKey(self,ent):
    return ent.uniquename()
  # ##################################################################
  # REGEX we will use in content comparison
  regexC1 = re.compile('\/\/.*?\n')
  regexC2 = re.compile('\/\*.*?\*\/')
  regexADA = re.compile('\-\-.*?\n')
  regexFORTRAN = re.compile('\!.*?\n')
  regex3 = re.compile('[\s\r\n]+')
  # ##################################################################
  searchRelationship = "Call"
  searchKind = ""
  # ##################################################################
  # Our function to determine if the "Code" of an entity have changed
  # we use contents (if exist), otherwise we check the value.. 
  def isSameContent(self, AFTER_ent, BEFORE_ent):
    if not AFTER_ent:
        return False
    if not BEFORE_ent:
        return False
    AFTER_content = AFTER_ent.contents()
    BEFORE_content = BEFORE_ent.contents()
    # Filtering obvious mismatch
    if (not AFTER_content) and (not BEFORE_content): # We do not have contents. Do we have value?
        AFTER_value = AFTER_ent.value()
        BEFORE_value = BEFORE_ent.value()
        if (not AFTER_value) and (not BEFORE_value):
            return True
        if (not AFTER_value):
            return False
        if (not BEFORE_value):
            return False
        return AFTER_value == BEFORE_value
    if not AFTER_content:
        return False   
    if not BEFORE_content:
        return False
    language = AFTER_ent.language()
    # Removing comments in content 
    if language == "C++" or language == "C" or language == "C#" or language == "Java" or language == "Plm":    
      AFTER_content = self.regexC1.sub('',AFTER_content)
      AFTER_content = self.regexC2.sub('',AFTER_content)
      BEFORE_content = self.regexC1.sub('',BEFORE_content)
      BEFORE_content = self.regexC2.sub('',BEFORE_content)
    if language == "Fortran":
      AFTER_content = self.regexFORTRAN.sub('',AFTER_content)
      BEFORE_content = self.regexFORTRAN.sub('',BEFORE_content)        
    if language == "Ada" or language == "VHDL":
      AFTER_content = self.regexADA.sub('',AFTER_content)
      BEFORE_content = self.regexADA.sub('',BEFORE_content)
    # Removing blanks and eol
    AFTER_content = self.regex3.sub('',AFTER_content)
    BEFORE_content = self.regex3.sub('',BEFORE_content)      
    return (AFTER_content == BEFORE_content)
  # ##################################################################
  # Note: the cache will contains a map of dataset for all entities
  # using their id() as a key..
  cache = {} # cache for entities
  dataset = None  # The dataset we are trying to compute.
  # ##################################################################     
  def addReferenceToDataset(self,ref,tag,cache_relations):
    uniquename = ref.ent().uniquename()
    if not uniquename in cache_relations:
      cache_relations[uniquename] = {}
    ref_kindname = ref.kind().longname()
    data = cache_relations[uniquename]
    if not ref_kindname in data:
      data[ref_kindname] = {"BEFORE":[],"AFTER":[]}
    data = data[ref_kindname]
    data[tag].append(ref)
   
  def computeDataForEntity(self,AFTER_ent,MAX,targets=None):
    # Are we computed already? If yes, we quit
    if AFTER_ent.id() in self.cache:
      return self.cache[AFTER_ent.id()]
    data = {"AFTER_Entity":AFTER_ent,"BEFORE_Entity":None,"isComputed":True,"isAdded":False,"isRemoved":False,"isSameContent":True,"hasChangedInTree":False,"ListOfLinks":[],"MAX":MAX,"toTarget":False}
    if targets:
      data["toTarget"] = data["toTarget"] or (AFTER_ent.id() in targets)
    self.cache[AFTER_ent.id()] = data
    if (MAX == 0):
      data["isComputed"] = False
      return self.cache[AFTER_ent.id()]      
    MAX = MAX-1
    data["isComputed"] = True
    # Finding a BEFORE_ent
    BEFORE_ent = self.BEFORE_db.lookup_uniquename(AFTER_ent.uniquename())
    if not BEFORE_ent:
      data["isAdded"] = True
    else:
      data["BEFORE_Entity"] = BEFORE_ent
    # Checking if content is the same...
    data["isSameContent"] = self.isSameContent(AFTER_ent,BEFORE_ent)
    # Analyzing relationships that were added and removed... 
    # The cache is structure as [uniquename] => [ref_kindname] => ["BEFORE" or "AFTER"] => LIST OF REFS
    cache_relations = {}
    for ref in AFTER_ent.refs(self.searchRelationship,self.searchKind):
      self.addReferenceToDataset(ref,"AFTER",cache_relations)
    if BEFORE_ent:
      for ref in BEFORE_ent.refs(self.searchRelationship,self.searchKind):
        self.addReferenceToDataset(ref,"BEFORE",cache_relations)
    # Talling those relationships...
    for uniquename in cache_relations:
      # We prep the entities...
      # BUG HERE: With uniquename @lBUF_strlcpy@kBUF_strlcpy@f./source_code/crypto/buffer/buffer.c, I will get an entity 
      # with ID @lBUF_strlcpy@kBUF_strlcpy@f./source_code/crypto/buffer/buf_str.c (TO BE RESOLVED)
      BEFORE_Target = self.BEFORE_db.lookup_uniquename(uniquename)
      AFTER_Target = self.AFTER_db.lookup_uniquename(uniquename)
      #print(uniquename)
      #print(BEFORE_Target.uniquename())
      #print(AFTER_Target.uniquename())
      data_for_BEFORE_Target = None
      data_for_AFTER_Target = None
      if AFTER_Target:
        data_for_AFTER_Target = self.computeDataForEntity(AFTER_Target,MAX,targets)
        if BEFORE_Target: # we have before and after.
          data_for_BEFORE_Target = {"AFTER_Entity":AFTER_Target,"BEFORE_Entity":BEFORE_Target,"isComputed":False,"isAdded":False,"isRemoved":False,"isSameContent":data_for_AFTER_Target["isSameContent"],"hasChangedInTree":False,"ListOfLinks":[]}
          data["hasChangedInTree"] = data["hasChangedInTree"] or data_for_AFTER_Target["hasChangedInTree"] or (not data_for_AFTER_Target["isSameContent"])          
        else:
          data["hasChangedInTree"] = True
        if targets:
            data["toTarget"] = data["toTarget"] or data_for_AFTER_Target["toTarget"]
      else:
        if BEFORE_Target: # we only have a before
          data_for_BEFORE_Target = {"AFTER_Entity":AFTER_Target,"BEFORE_Entity":BEFORE_Target,"isComputed":False,"isAdded":False,"isRemoved":True,"isSameContent":False,"hasChangedInTree":False,"ListOfLinks":[]}   
          data["hasChangedInTree"] = True
        if targets:
          data["toTarget"] = False
      for ref_kindname in cache_relations[uniquename]:        
        newLink = { "Reference Kind":ref_kindname,"ListOfBEFORE_Refs":cache_relations[uniquename][ref_kindname]["BEFORE"],\
                                                  "ListOfAFTER_Refs":cache_relations[uniquename][ref_kindname]["AFTER"],"isChanged":False,"isAdded":False,"isRemoved":False,\
                                                  "targetdata_BEFORE":None, "targetdata_AFTER":None, "toTarget":False }
        newLink["isAdded"] = len(newLink["ListOfBEFORE_Refs"]) == 0
        newLink["isRemoved"] = len(newLink["ListOfAFTER_Refs"]) == 0
        newLink["isChanged"] = (not newLink["isAdded"]) and (not newLink["isRemoved"]) and len(newLink["ListOfAFTER_Refs"]) != len(newLink["ListOfBEFORE_Refs"])
        if targets:
          if data_for_AFTER_Target:
            newLink["toTarget"] = data_for_AFTER_Target["toTarget"]
          else:
            newLink["toTarget"] = False
        data["hasChangedInTree"] = data["hasChangedInTree"] or (newLink["isAdded"] or newLink["isRemoved"] or newLink["isChanged"])
        data["ListOfLinks"].append(newLink)                      
        newLink["targetdata_BEFORE"] = data_for_BEFORE_Target
        newLink["targetdata_AFTER"] = data_for_AFTER_Target    
    return data
  # ##################################################################    
  def computeData(self,ent,MAX,targets=None): # Compute for an entity. MAX=-1 for no limit.
    # Reset the DATASET
    self.cache = {}
    self.computeDataWithoutCacheCleaning(ent,MAX,targets)
  def computeDataWithoutCacheCleaning(self,ent,MAX,targets=None):
    # Reset the DATASET
    self.dataset = None  
    if not self.AFTER_db or not self.BEFORE_db:
      return
    self.dataset = self.computeDataForEntity(ent,MAX,targets)    
  # ##################################################################
  def computeMetricsInternal(self,dataset):
    result = { "NbAddedLink":0,"NbRemovedLink":0,"NbChangedLink":0,"NbItemChanged":0,"NbItemTotal":0,"NbNonComputed":0 }   
    if not dataset:
        return result 
    # ##################################################################  
    key="KEY"  
    if dataset["AFTER_Entity"]:
      key=key+str(dataset["AFTER_Entity"].id())+","
    if dataset["BEFORE_Entity"]:
      key=key+str(dataset["BEFORE_Entity"].id())
    # ##################################################################   
    if key in self.cacheDraw:
      return result; # Already counted for, no need to add...
    self.cacheDraw[key] = True
    # ##################################################################
    if not dataset["isRemoved"]:    
      result["NbItemTotal"] = 1    
      if not dataset["isSameContent"]:
        result["NbItemChanged"] = 1     
      if not dataset["isComputed"]:
        result["NbNonComputed"] = 1 
    for link in dataset["ListOfLinks"]:
      if link["isAdded"]:
        result["NbAddedLink"] = result["NbAddedLink"] + 1 
      if link["isRemoved"]:
        result["NbRemovedLink"] = result["NbRemovedLink"] + 1
      if link["isChanged"]:
        result["NbChangedLink"] = result["NbChangedLink"] + 1
      if link["targetdata_AFTER"]:
        result_next = self.computeMetricsInternal(link["targetdata_AFTER"])  
      else:
        result_next = self.computeMetricsInternal(link["targetdata_BEFORE"]) 
      result["NbAddedLink"] = result["NbAddedLink"] + result_next["NbAddedLink"]
      result["NbRemovedLink"] = result["NbRemovedLink"] + result_next["NbRemovedLink"]
      result["NbChangedLink"] = result["NbChangedLink"] + result_next["NbChangedLink"]
      result["NbItemChanged"] = result["NbItemChanged"] + result_next["NbItemChanged"]
      result["NbItemTotal"] = result["NbItemTotal"] + result_next["NbItemTotal"]
      result["NbNonComputed"] = result["NbNonComputed"] + result_next["NbNonComputed"]
    # ##################################################################      
    return result
  # ##################################################################
  def printDataSetInternal(self,tab,dataset):
    if not dataset:
      print(tab+"No DATASET")
      return
    # ##################################################################  
    key="KEY"  
    if dataset["AFTER_Entity"]:
      key=key+str(dataset["AFTER_Entity"].id())+","
    if dataset["BEFORE_Entity"]:
      key=key+str(dataset["BEFORE_Entity"].id())
    # ##################################################################  
    if dataset["AFTER_Entity"]:
      #print(tab+self.getFullDescriptionOfFunction(dataset["AFTER_Entity"]))
      print(tab+dataset["AFTER_Entity"].longname()+"(...)",end="")
    else:
      if dataset["BEFORE_Entity"]:
        #print(tab+self.getFullDescriptionOfFunction(dataset["BEFORE_Entity"]))
        print(tab+dataset["BEFORE_Entity"].longname()+"(...)",end="")
    # ##################################################################  
    if key in self.cacheDraw:
      print(" [<-already shown]")
      return;
    self.cacheDraw[key] = True
    # ##################################################################  
    if dataset["isAdded"]:
      print(" ADDED",end="")
    if dataset["isRemoved"]:
      print(" REMOVED",end="")
    if not dataset["isSameContent"]:
      print(" CHANGED",end="")
    if dataset["hasChangedInTree"]:
      print(" TREE",end="")      
    if not dataset["isComputed"]:
      print(" [STOP]",end="")
    if "toTarget" in dataset:
      if dataset["toTarget"]:
        print("(*)",end="")
    print("")
    for link in dataset["ListOfLinks"]:
      print(tab+"  =="+link["Reference Kind"]+"=",end="")
      if "toTarget" in link:
        if link["toTarget"]:
          print("(*)",end="")
      if link["isAdded"]:
        print("ADDED",end="")
      if link["isRemoved"]:
        print("REMOVED",end="")
      if link["isChanged"]:
        print("CHANGED("+str(len(link["ListOfBEFORE_Refs"]))+" to "+str(len(link["ListOfAFTER_Refs"]))+")",end="")
      print("=>")
      if link["targetdata_AFTER"]:
        self.printDataSetInternal(tab+"     ",link["targetdata_AFTER"])           
      else:
        self.printDataSetInternal(tab+"     ",link["targetdata_BEFORE"])
  # ################################################################## 
  cacheDraw = {}
  # ##################################################################
  # Print the internal dataset (for debug)
  def printDataSet(self):
    self.cacheDraw = {}
    self.printDataSetInternal("",self.dataset)
  # ##################################################################
  # Compute Metrics related to the tree that was created.
  # Result is a map containing:
  # "NbAddedLink":
  # "NbRemovedLink":
  # "NbChangedLink":
  # "NbItemChanged":
  # "NbItemTotal":
  # "NbNonComputed":
  # Note: DO NOT USE: Metrics has to be reviewed to see what we really
  # want to see. May be ok as a removed node will not have any more links..
  def computeMetrics(self):
    self.cacheDraw = {}
    return self.computeMetricsInternal(self.dataset)    
  # ################################################################## 
  def __init__(self, db):
    self.AFTER_db = db
    if not self.AFTER_db:
      return
    self.BEFORE_db = db.comparison_db()
    if not self.BEFORE_db:
      self.BEFORE_db = self.AFTER_db
  # ################################################################## 
  def setComparisonDb(self, beforedb): # Optional
    self.BEFORE_db = beforedb
    if not self.BEFORE_db:
      self.BEFORE_db = self.AFTER_db
  # ################################################################## 
  def setSearchOptions(self, relationship, kind): # Optional
    self.searchRelationship = relationship
    self.searchKind = kind
  # ################################################################## 
    
      
      
      
      
