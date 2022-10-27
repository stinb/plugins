# ##################################################################
import understand
import sys
import re
import os
import traceback
# ##################################################################
# Please contact stephane.raynaud@Emenda.com for help/information
# This file is provided AS-IS
# ##################################################################
# ArchitectureParser is a class to help you parse and manipulate
# architecture.
#
# [1] findArchitecturesWithName(name,recursive=False):
#   - Returns an array of Architecture objects with name that
#     "includes" name (case insensitive!)
#   - if a match is found, its children will be searched only if recursive is True
#
# [2] createListOfObjectFromDescriptionArchitecture(arch)
#   - This returns a {}
#       . The index is entity ids
#       .At each index, the value is an array containing:
#           . the entity
#           . the reason why it was added
#   - The architecture is parsed the same way you would read it...
#       . an entity at root is included
#       . then it's by folder. Examples:
#            .*/call/function would mean "any entity that call a function"
#            .function/use/[Ent=stdin] would mean "function that uses stdin"
#            .a * is "all kind"
# ##################################################################
class ArchitectureParser:
    # ##################################################################
    # findArchitecturesWithName ########################################
    # ##################################################################
    def findArchitecturesWithNameINTERNAL(self,archs,name,recursive):
        result = []
        if archs:
            for arch in archs:
                doChildren = True
                if name in arch.name().upper():
                    result.append(arch)
                    if not recursive:
                        doChildren = False
                if doChildren:
                    sub_result = self.findArchitecturesWithNameINTERNAL(arch.children(),name,recursive)
                    result = result + sub_result
        return result                                                        
    def findArchitecturesWithName(self,name,recursive=False):
        return self.findArchitecturesWithNameINTERNAL(self.db.root_archs(),name.upper(),recursive)        
    # ##################################################################
    # createListOfObjectFromDescriptionArchitecture ####################
    # ##################################################################
    # listofInvertReferences
    # return an array of all the opposite references from a reference name
    def listofInvertReferences(self,ref_name):
        result = []
        relationsOfKindrelation = understand.Kind.list_reference(ref_name)
        for relationOfKindrelation in relationsOfKindrelation:
            reverseRelationkind = relationOfKindrelation.inv()
            result.append(reverseRelationkind.longname())
        return result
    # getEntitiesThruReferenceFromEntities
    # get list of Entities from list of Entities and list of Reference String
    # only of a specific filter
    def getEntitiesThruReferenceFromEntities(self,original_reference,reference_names,filters,entities):
        result = {}
        for reference_name in reference_names:
            for entity in entities:
                referenced_entities = entities[entity][0].ents(reference_name,filters)
                for referenced_entity in referenced_entities:
                    if not referenced_entity.id() in result:
                        result[referenced_entity.id()] = [referenced_entity," it '"+original_reference+"'"+entities[entity][1]]
        return result
    # listOfEntitiesInSingleArchitecture_INTERNAL
    # Add the entities specified in an architecture manually..
    def listOfEntitiesInSingleArchitecture_INTERNAL(self,arch):
        result = {}
        elementsToAdd = arch.ents(False)
        for elementToAdd in elementsToAdd:
            if not elementToAdd.id() in result:
                result[elementToAdd.id()] = [elementToAdd," '"+elementToAdd.name()+"'"]
        return result
    def seekFromReference_INTERNAL(self,arch,filters):
        result = {}
        reference_name = arch.name()
        invert_reference_names = self.listofInvertReferences(reference_name)
        targets_manual = self.listOfEntitiesInSingleArchitecture_INTERNAL(arch)
        result = self.getEntitiesThruReferenceFromEntities(reference_name,invert_reference_names,filters,targets_manual) 
        for child_arch in arch.children():
            targets_auto = self.seekFromKind_INTERNAL(child_arch)
            xtra_result = self.getEntitiesThruReferenceFromEntities(reference_name,invert_reference_names,filters,targets_auto)
            for index in xtra_result:
               if not index in result:
                    result[index] = [xtra_result[index][0],xtra_result[index][1]] 
        return result
    def seekFromKind_INTERNAL(self,arch):
        result = {}
        filter_kind = ""
        reason = " is an entity"
        if arch.name() != "*":
            filter_kind = arch.name()
            reason = " is a '"+filter_kind+"'"
        if len(arch.children()) == 0: # We need to find all...
            ents = self.db.ents(filter_kind)
            for ent in ents:
                result[ent.id()] = [ent,reason]
        else: # we are going to use some references.
            for child_arch in arch.children():
                results_auto = self.seekFromReference_INTERNAL(child_arch,filter_kind)
                for index in results_auto:
                    if not index in result:
                        result[index] = [results_auto[index][0],reason+" (namely '"+results_auto[index][0].name()+"')"+results_auto[index][1]]
        return result        
    def createListOfObjectFromDescriptionArchitecture(self,arch):
        result = {}
        results_manual = self.listOfEntitiesInSingleArchitecture_INTERNAL(arch)
        for index in results_manual:
            result[index] = [results_manual[index][0],results_manual[index][1]]                
        for child_arch in arch.children():
            results_auto = self.seekFromKind_INTERNAL(child_arch)
            for index in results_auto:
                if not index in result:
                    result[index] = [results_auto[index][0],results_auto[index][1]]
        return result                            
    # ##################################################################
    def __init__(self, db):
        self.db = db
    # ##################################################################
