# ##################################################################
import understand
import sys
import re
import os
import traceback
import collections
# ##################################################################
#sys.path.insert(0, '.')
sys.path.insert(0, '___D___')
import comparison
# ##################################################################
# Please contact stephane.raynaud@Emenda.com for help/information
# This file is provided AS-IS
# ##################################################################
# Pather is a class to help you determine path(s) from a set of functions
# to another set of functions. It can be used to determine:
#   - all the path(s)
#   - all the path(s) with more than one change on them
#   - list of the changes in all the path(s)
# Note:
#   - a change is that either the "function" has changed (isSameContent=False)
#     or that the list of calls it make has changed somewhat.
#   - The first and last function on a path is not considered for change
#     (if you start with a 'changed' function, we don't want to hightlight all paths)
#
# USAGE:
#   [1] p = pather.Pather(db)
#   [2] p.computePaths(from_ents,to_ents)
#           where from_ents: [] of entities
#           where to_ents: [] of entitites
#   then, you can
#       - getAllPaths: return all possible path(s)
#       - getChangedPaths: return only the paths with a change
#       - getChangeActors: return all the functions that are changed and impact paths
# ##################################################################
class Pather:
    # ##################################################################
    # Convert Path to a tree.
    # A Tree is a [[START1,children1,parent1],[START2,children2,parent2],....]
    # Where START is the [A0,B0,C0,D0] from paths.
    # A(0):ent B(1):isChange C(2):isStart D(3):isEnd
    # The children are a [] or [[NODE1,children1,parent1],[NODE2,children2,parent2],....]
    # where NODE is the [A0,B0,C0,D0] for that node.
    # Note: This conversion create the possibility of infinite loop when going down the tree...
    def convertPathsToTree(self,paths):
        cache = {}
        result = []        
        for path in paths:
            lastindex = len(path)
            c_path_elt = path[0] # element on path.
            if not c_path_elt[0].id() in cache:
                cache[c_path_elt[0].id()] = [c_path_elt,[],[]]                
                if c_path_elt[2]: # we only add start element 1 time to the "root"
                    result.append(cache[c_path_elt[0].id()])
            c_node = cache[c_path_elt[0].id()] # element in the tree.            
            for index in range(1,lastindex):
                n_path_elt = path[index]
                if not n_path_elt[0].id() in cache:
                    cache[n_path_elt[0].id()] = [n_path_elt,[],[]]
                n_node = cache[n_path_elt[0].id()]
                # ==============================================
                pleaseAddIt = True  # we only add 1 element one time to its parent
                                    # Note that we keep going in the line as the children might be different here.
                                    # We can't simply stop.
                for testElement in c_node[1]:
                    if testElement[0][0].id() == n_path_elt[0].id():
                        pleaseAddIt = False
                        break
                if pleaseAddIt:
                    c_node[1].append(n_node)
                # ==============================================
                pleaseAddIt = True  
                for testElement in n_node[2]:
                    if testElement[0][0].id() == c_path_elt[0].id():
                        pleaseAddIt = False
                        break
                if pleaseAddIt:
                    n_node[2].append(c_node)
                # ==============================================
                c_node = n_node
                c_path_elt = n_path_elt
        return result        
    # ##################################################################
    # To prevent recomputation, we will use caches...
    cache_all_paths = None
    cache_all_changed_paths = None
    # ##################################################################
    # Internal seek. result contains an [[A0,B0,C0,D0],[A1,B1,C1,D1],[A2,B2,C2,D2],....]
    # where As are the entity and Bs are True|False indicating a change.
    # Cs is whether they were a start and Ds is whether they were a end.
    # start is the starting entity
    # outs is a [] of ids for the end points entities
    def seek(self,dataset,start,outs,pipeline,pipeline_cache,pipelineHasChanged,result,changeOnly):
        if not dataset:
            return
        if dataset["AFTER_Entity"]:
            AFTER_ent = dataset["AFTER_Entity"]
            AFTER_id = AFTER_ent.id()
            isOut = (AFTER_id in outs)
            isFinal = (AFTER_id in pipeline_cache)
            entityHasChanged = dataset["isAdded"] or dataset["isRemoved"] or (not dataset["isSameContent"])
            for link in dataset["ListOfLinks"]:
                entityHasChanged = entityHasChanged or (link["isAdded"] or link["isRemoved"] or link["isChanged"])   
            pipeline.append([AFTER_ent,entityHasChanged,start.id()==AFTER_id,isOut])
            pipeline_cache.append(AFTER_id)
            if isOut: # This is a final node, we need to flush it!
                if pipelineHasChanged: # we DID NOT update change for an out at the end of pipeline.
                    result.append(pipeline)
                else:
                    if not changeOnly:
                        result.append(pipeline)
            if isFinal: # We are done, we already seen this one in the same stream.
                return
            if not (start.id() == dataset["AFTER_Entity"].id()): # In case this is NOT the 1st step...
                pipelineHasChanged = entityHasChanged or pipelineHasChanged
            # Now we are going to the next one...
            for link in dataset["ListOfLinks"]:
                if link["toTarget"]: 
                  localpipeline = pipeline.copy()
                  localpipeline_cache = pipeline_cache.copy()
                  self.seek(link["targetdata_AFTER"],start,outs,localpipeline,localpipeline_cache,pipelineHasChanged,result,changeOnly)

    # ##################################################################
    # getPaths: return the path possible during computation
    # You can limit the path by limiting the from(s) and the to(s)
    # from_subset and to_subset are [] of entities.
    # result contains an [[A0,B0,C0,D0],[A1,B1,C1,D1],[A2,B2,C2,D2],....]
    # where As are the entity and Bs are True|False indicating a change.
    # Cs is whether they were a start and Ds is whether they were a end.
    # and Bs are True|False indicating a change.
    # NOTE: Change(s) in the from or in the to entity are not considered to consider if "changed"
    def getPaths(self,changeOnly,from_subset=None,to_subset=None):
        # =======================================================
        if from_subset == None:
            from_subset = self.from_ents
        if to_subset == None:
            to_subset = self.to_ents
        # =======================================================        
        # Is this using all the "ins" and "outs" we computed?
        isAGlobalSet = (set(from_subset)==set(self.from_ents)) and (set(to_subset)==set(self.to_ents))
        # =======================================================
        # use Caching if possible
        if isAGlobalSet:
            if changeOnly:
                if self.cache_all_changed_paths != None:
                    return self.cache_all_changed_paths.copy()
            else:
                if self.cache_all_paths != None:
                    return self.cache_all_paths.copy()
        else:
            if changeOnly:
                if self.cache_all_changed_paths != None:
                    result = []
                    for path in self.cache_all_changed_paths:
                        if (path[0][0] in from_subset) and (path[-1][0] in to_subset):
                            result.append(path)
                    return result
            else:
                if self.cache_all_paths != None:
                    result = []
                    for path in self.cache_all_paths:
                        if (path[0][0] in from_subset) and (path[-1][0] in to_subset):
                            result.append(path)
                    return result
        # =======================================================
        outs = []
        for ent in to_subset:
            outs.append(ent.id())
        # =======================================================
        result = []
        for start in from_subset:
            dataset = self.comparisonLoader.cache[start.id()]
            pipeline = []
            pipeline_cache = []
            one_result = []
            self.seek(dataset,start,outs,pipeline,pipeline_cache,False,one_result,changeOnly)
            result = result + one_result
        # =======================================================
        # We cache the results (if all the ins and outs were in the computation):
        if isAGlobalSet:
            if changeOnly:
                if self.cache_all_changed_paths == None:
                    self.cache_all_changed_paths = result.copy()
            else:
                if self.cache_all_paths == None:
                    self.cache_all_paths = result.copy()
        # =======================================================
        return result
    # ##################################################################
    # getAllPaths: return all the path possible during computation
    def getAllPaths(self,from_subset=None,to_subset=None):
        return self.getPaths(False,from_subset,to_subset)
         
    # getAllPaths: return all the path possible during computation (with changes)
    # NOTE: Change(s) in the from or in the to entity are not considered to consider if "changed"
    def getChangedPaths(self,from_subset=None,to_subset=None):
        return self.getPaths(True,from_subset,to_subset)

    # getChangeActors: return all the functions that are changed and impact paths
    # You can limit the path by limiting the from(s) and the to(s)
    # from_subset and to_subset are [] of entities.
    # return a [[A0,B0,C0,D0,E0],[A1,B1,C1,D1,E1],[A2,B2,C2,D2,E2],....]
    # where As are the entity and Bs are True|False indicating a change.
    # Cs is whether they were a start and Ds is whether they were a end.
    # and Bs are True|False indicating a change.
    # Es is the number of path impacted by the entity
    # DOES NOT RETURN START OR STOP !!!
    def getChangeActors(self,from_subset=None,to_subset=None):
        if from_subset == None:
            from_subset = self.from_ents
        if to_subset == None:
            to_subset = self.to_ents           
        temp_result = self.getPaths(True,from_subset,to_subset)
        result = []
        result_ids = {}
        for path in temp_result:
            for element in path:
                if element[1]: # hasChanged
                    if not(element[0] in from_subset) and not(element[0] in to_subset):     
                        if not element[0].id() in result_ids:
                            data = element.copy()
                            data.append(1) # 1 path!
                            result.append(data)
                            result_ids[element[0].id()] = len(result)-1
                        else:
                            result[result_ids[element[0].id()]][4] += 1
        return result        
    # getChangedEndpoints: return all the start or end that have changed.
    # return a [[A0,B0,C0,D0,E0],[A1,B1,C1,D1,E1],[A2,B2,C2,D2,E2],....]
    # where As are the entity and Bs are True|False indicating a change.
    # Cs is whether they were a start and Ds is whether they were a end.
    # and Bs are True|False indicating a change.
    # Es is the number of path impacted by the entity
    def getChangedEndpoints(self,from_subset=None,to_subset=None):
        if from_subset == None:
            from_subset = self.from_ents
        if to_subset == None:
            to_subset = self.to_ents           
        temp_result = self.getPaths(True,from_subset,to_subset)
        result = []
        result_ids = {}
        for path in temp_result:
            for element in path:
                if element[1]: # hasChanged
                    if (element[0] in from_subset) or (element[0] in to_subset):     
                        if not element[0].id() in result_ids:
                            data = element.copy()
                            data.append(1) # 1 path!
                            result.append(data)
                            result_ids[element[0].id()] = len(result)-1
                        else:
                            result[result_ids[element[0].id()]][4] += 1
        return result 
    # ##################################################################
    # Flush the internal computation Caches
    def flushCache(self):
        self.cache_all_paths = None
        self.cache_all_changed_paths = None
    # ##################################################################
    # from_ents: [] of entities
    # to_ents: [] of entitites
    def computePaths(self,from_ents,to_ents):
        self.cache_all_paths = None
        self.cache_all_changed_paths = None
        self.from_ents = from_ents
        self.to_ents = to_ents
        # We only clean cash on 1st run, so we need to keep track of where we are
        isFirstLoop = True
        # We need an array of id(s) to know where we stop.
        array_of_tos = [] # list of ids of terminal functions.
        for to_ent in to_ents:
            array_of_tos.append(to_ent.id())
        # we compute all the needed comparisons
        for from_ent in from_ents:
            if isFirstLoop:
                self.comparisonLoader.computeData(from_ent,-1,array_of_tos)
                isFirstLoop = False
            else:
                self.comparisonLoader.computeDataWithoutCacheCleaning(from_ent,-1,array_of_tos)     
    # ##################################################################
    def __init__(self, db):
        self.cache_all_paths = None
        self.cache_all_changed_paths = None
        self.db = db
        self.comparisonLoader = comparison.Loader(db)
        self.from_ents = []
        self.to_ents = []
    # ##################################################################
