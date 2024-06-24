#
# Synopsis: Report unused entities
#
# Categories: Coding Standards, Project Report
#
# Languages: Ada, Fortran, C, Java
#
# Description:
#  Reports unused entities, sorted either by filename (default) or by
#  entity kind.  The file/line location of the definition is also reported.  
#  When run within Understand (from Options->Run A Command), double click 
#  on the output line to visit the entity definition in the source. 
#  Requires an existing Understand database.
#
#
# Written by Jason Quinn
#
# 1/26/21

import understand
import sys
import re
import argparse

# Usage:
parser = argparse.ArgumentParser()
parser.add_argument("db", help="Specify Understand database")
parser.add_argument("-kind", help="(optional) Specify a kind text string. For instance Enumerator.")
args = parser.parse_args()

# Get the declaration ref
def getDeclRef(ent):
    decl = None

    # Language specific
    if re.search('ada', ent.language()):
        decl = ent.ref("declarein ~spec ~body ~instance ~formal ~incomplete ~private ~stub")
        if not decl:
            decl = ent.ref("spec declarein")
        if not decl:
            decl = ent.ref("body declarein")
        if not decl:
            decl = ent.ref("instance declarein")
        if not decl:
            decl = ent.ref("formal declarein")
        if not decl:
            decl = ent.ref("incomplete declarein")
        if not decl:
            decl = ent.ref("private declarein")
        if not decl:
            decl = ent.ref("stub declarein")

    # Fortran, Java, and C\C++
    else:
        decl = ent.ref("definein")
        if not decl:
            decl = ent.ref("declarein")

    return decl

# Check if it is a duplicate ref
def duplicateRef(ref1, ref2):
    if not (ref1 and ref2):
        return False

    if (ref1.file().longname() == ref2.file().longname()) and (ref1.line() == ref2.line()) and (ref1.column() == ref2.column()):
        return True

    return False

if __name__ == '__main__':
    # Open Database
    db = understand.open(args.db)

    if args.kind:
        entKinds = args.kind
    else:
        entKinds = """ada object ~task ~unknown ~unresolved,\
	ada constant ~unknown ~unresolved,\
	ada parameter,\
	ada function ~unknown ~unresolved,\
	ada procedure ~unknown ~unresolved,\
	ada type ~unknown ~unresolved,\
	fortran variable,\
	fortran function ~unresolved ~intrinsic,\
	fortran subroutine ~unresolved ~instrinsic,\
	fortran type ~unknown,\
	c object ~unknown ~unresolved,\
	c parameter,\
	c function ~unknown ~unresolved,\
	c type ~unknown ~unresolved ~unnamed,\
	java variable,\
	java parameter,\
	java method,\
	java class ~anonymous,\
	java interface"""



    ents = db.ents(entKinds)
    unused = []
    for ent in ents:
        if re.search('standard', ent.library()) or ent.refs("ada rename") or ent.refs("java definein", "java abstract method") or (ent.kind().check("java method") and ent.name() == "main"):
            continue
        # Filter out ents with the following refs
        if not ent.refs("""associationby,\
                        base,\
                        callby ~inactive,\
                        derive,\
                        java callby,\
                        java castby,\
                        java createby,\
                        java dotrefby,\
                        java extendby,\
                        java implementby,\
                        java modifyby,\
                        java override,\
                        java setby ~init,\
                        java throwby,\
                        java typedby,\
                        java useby,\
                        derivefrom,\
                        instanceof,\
                        modifyby,\
                        refby,\
                        renameby,\
                        subtypefrom,\
                        typedby,\
                        useby ~inactive,\
                        usepackageby,\
                        usetypeby,\
                        withby"""):

            declRef = getDeclRef(ent)

            # Skip over unresolved entities
            if not declRef:
                continue

            # If not used, check for setbys - may be set where defined but otherwise unused
            setOnDecl = False
            setByRef = []
            if ent.kind().check("object, parameter, variable"):
                setByRef = ent.refs("Setby")
                if len(setByRef) == 1:
                    if duplicateRef(setByRef[0], declRef):
                        setOnDecl = True

            # If there are no setby references, or only one at the point of definition, call it unused
            if len(setByRef) < 1 or setOnDecl:
                unused.append(declRef)


    # Sort unused by kind and print
    if args.kind:
        lastKind = ""
        unused.sort(key=lambda x: x.file().longname())
        for ref in unused:
            thisKind = ref.scope().kindname()

            if thisKind != lastKind:
                print(f'\n{thisKind}')
                lastKind = thisKind

            print('\t', end='')
            print(f'{ref.scope().longname()}', end='')
            print('\t', end='')
            print(f'[File: {ref.file().longname()} Line: {ref.line()}', end='')
            endRef = ref.scope().refs("end")
            if endRef:
                print(f'-{endRef[0].line()}')

            else:
                print()

    # Sort unused by file and print
    else:
        lastFile = ""
        unused.sort(key=lambda x: x.file().longname())
        for ref in unused:
            thisFile = ref.file().longname()


            if thisFile != lastFile:
                print(f'\n{thisFile}')
                lastFile = thisFile

            print('\t', end='')
            print(f'{ref.scope().longname()}', end='')
            print('\t', end='')
            print(f'[File: {ref.file().longname()} Line: {ref.line()}', end='')

            endRef = ref.scope().refs("end")

            if endRef:
                print(f'-{endRef[0].line()}')

            else:
                print()


