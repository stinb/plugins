#
# Synopsis: Report dead code
#
# Categories: Coding Standards, Project Report
#
# Languages: C, C++, Ada, Fortran, Java
#
# Description:
#   Report dead code. That is, report any functions or
#   other subprograms that are not directly or indirectly
#   called by one of the entities specified as used. If a
#   complete program is analyzed, this entity should be the
#   "main" entity.
#
# Written by Jason Quinn
#
# 1/22/21

import understand
import sys
import re
import argparse

# Usage:
parser = argparse.ArgumentParser()
parser.add_argument("db", help="Specify Understand database")
parser.add_argument("-root", help="""(optional)a comma separated list of root \
        function known to be used. If not specified then main will be used by default""")
args = parser.parse_args()


if __name__ == '__main__':
    # Open Database
    db = understand.open(args.db)

    # Split the user inputed arguments and put them into a list
    root = args.root
    if root:
        if re.search(', ', root):
            roots = root.split(', ')
        elif re.search(',', root):
            roots = root.split(',')
        else:
            roots = root

    else:
        roots = 'main'

    # Gather all functions that aren't unresolved and not unknown
    funcs = db.ents("function ~unresolved ~unknown, method ~unresolved ~unknown, procedure ~unresolved, program, subroutine ~unresolved")
    for func in funcs:
        if re.search('standard', func.library().lower()) or func.ref("instanceof"):
            continue
        if re.search('c', func.language().lower()) and func.search('~|operator', func.name().lower()):
            continue
        if re.search('fortran', func.language().lower()) and func.ref("UseRenameEntityby"):
            continue
        # Print function name if there aren't any callby references and the user did
        # not specify the function in the arguments
        
        if (re.search('c', func.language().lower()) or re.search('fortran', func.language().lower())) and not func.refs("callby ~inactive, useby ptr ~inactive") and func.longname() not in roots:
            print(func.longname())

        elif (re.search('ada', func.language().lower()) or re.search('java', func.language().lower())) and not func.refs("call ~inactive, use ~inactive") and func.longname() not in roots:
            print(func.longname())

