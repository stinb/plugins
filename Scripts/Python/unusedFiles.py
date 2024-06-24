# Sample Understand perl API program 
#
# Synopsis: Report unused files 
#
# Categories: Project Report, Coding Standards
#
# Languages: All
#
# Written by Jason Quinn
#
# 1/28/21
#
import understand
import sys
import re
import argparse

# Usage:
parser = argparse.ArgumentParser()
parser.add_argument("db", help="Specify Understand database")
args = parser.parse_args()

if __name__ == '__main__':
    # Open Database
    db = understand.open(args.db)

    filesUsed = {}

    files = db.ents("File ~undefined ~unresolved")

    files.sort(key=lambda x: x.longname())
    for file in files:
        print(f'{file.longname()}\n')

        id = file.id()
        filesUsed[id] = {}
        filesUsed[id]['ent'] = file
        filesUsed[id]['externalRefs'] = 0

        refs = file.refs()

        for ref in refs:
            ent = ref.ent()

            useRefs = ent.refs("setby,declarein,definein,call,use,useby,call,callby,includeby,withby,renameby")

            for useRef in useRefs:
                useFile = useRef.file().longname()
                if useFile != file.longname():
                    filesUsed[id]['externalRefs'] += 1

    keys = filesUsed.keys()

    for key in keys:
        ent = filesUsed[key]['ent']
        externalRefs = filesUsed[key]['externalRefs']


        if not externalRefs and (ent.library() != "Standard"):
            print(f'unused file: {ent.name()}\n')

