# Sample Understand Python API program 
#
# Synopsis: Report Halstead Complexity for each function/file in project
#
# Language: Ada, C/C++, Java, Fortran
#
# Description:
#  Report Halstead Complexity for each function/file in project. 
#  See http://www.sei.cmu.edu/activities/str/descriptions/halstead_body.html
#  Any identifier, constant, or string is considered an operand. Any operator
#  or keyword or punctuation (except for )}]) is considered an operator.
#  Specify the -comma option to report in a comma-delimited format for 
#  importing into a spreadsheet.
#  Requires an existing Understand database
#
# Written by Jason Quinn
#
# 1/25/21

import understand
import sys
import re
import argparse
import math

# Usage:
parser = argparse.ArgumentParser()
parser.add_argument("db", help="Specify Understand database")
parser.add_argument("-comma", help="(optional) Specify for comma delimited format which can be used in a spreadsheet.")
args = parser.parse_args()

# Obtains function begin and end lines
def getFunctionDefnLines(func):
    begin_ref = func.ref("definein")
    end_ref = func.ref("end")
    if not begin_ref or not end_ref:
        return None
    return (begin_ref.line(), end_ref.line())

def getKinds(file):
    fileLanguage = file.language().lower()
    if re.search('ada', fileLanguage):
        refKind = "declarein body"
        entKind = "function, procedure"
    elif re.search('java|c#', fileLanguage):
        refKind = "definin"
        entKind = "method"
    elif re.search('fortran', fileLanguage):
        refKind = "definein"
        entKind = "fortran subroutine, fortran function, fortran main program"
    else:
        refKind = "Define"
        entKind = "C function"

    return refKind, entKind



# Scan each separate function and return the 4 basic operator/operand metrics
def scanEntity(lexer, func):
    n1 = 0
    n2 = 0
    N1 = 0
    N2 = 0

    n1Dict = {}
    n2Dict = {}

    begin, end = getFunctionDefnLines(func.ent())

    lexemes = lexer.lexemes(begin, end)
    
    for lexeme in lexemes:
        if lexeme.token() == "Operator" or lexeme.token() == "Keyword" or lexeme.token() == "Punctuation":
            if not re.search('[)}\]]', lexeme.text()):
                n1Dict[lexeme.text()] = 1
                N1 += 1

        elif lexeme.token() == "Identifier" or lexeme.token() == 'Literal' or lexeme.token() == "String":
            n2Dict[lexeme.text()] = 1
            N2 += 1

    n1 = len(n1Dict.keys())
    n2 = len(n2Dict.keys())

    return n1, n2, N1, N2

if __name__ == '__main__':
    # Open Database
    db = understand.open(args.db)
    files = db.ents("file ~unknown ~unresolved")

    # Initialize the project_metric dictionary
    project_metric = {}
    project_metric['n1'] = 0
    project_metric['n2'] = 0
    project_metric['N1'] = 0
    project_metric['N2'] = 0
    project_metric['N'] = 0
    project_metric['n'] = 0
    project_metric['V'] = 0
    project_metric['D'] = 0
    project_metric['E'] = 0

    # Create the separator
    if args.comma:
        sep = ','
    else:
        sep = '\t'

    # Print the labels
    print(f'Entity Name{sep}n1{sep}n2{sep}N1{sep}N2{sep}Len.{sep}Voc.{sep}Vol.{sep}Dif.{sep}Eff.\n\n')
    # Sort and iterate through the files
    files.sort(key=lambda x: x.longname())
    for file in files:
        lexer = file.lexer()
        refKind, entKind = getKinds(file)
        funcs = file.filerefs(refKind, entKind)
        if not (lexer and funcs):
            continue

        # Initialize the file_metric dictionary
        file_metric = {}

        file_metric['n1'] = 0
        file_metric['n2'] = 0
        file_metric['N1'] = 0
        file_metric['N2'] = 0
        file_metric['N'] = 0
        file_metric['n'] = 0
        file_metric['V'] = 0
        file_metric['D'] = 0
        file_metric['E'] = 0

        funcs.sort(key=lambda x: x.ent().name())
        for func in funcs:
            n1, n2, N1, N2 = scanEntity(lexer, func)

            # Calculate the data
            n, N = (n1 + n2, N1 + N2)
            if n:
                V = int(N * (math.log(n) / math.log(2)))
            else:
                V = 0

            if n2:
                D = int((n1 / 2) * (N2 / n2))
            else:
                D = 0

            E = D * V

            # Print metrics for this function
            print(sep.join([func.ent().name(), str(n1), str(n2), str(N1), str(N2), str(N), str(n), str(V), str(D), str(E)]))
            
            # Add the function metrics to the file_metric dictionary
            file_metric['n1'] += n1
            file_metric['n2'] += n2
            file_metric['N1'] += N1
            file_metric['N2'] += N2
            file_metric['N'] += N
            file_metric['n'] += n
            file_metric['V'] += V
            file_metric['D'] += D
            file_metric['E'] += E

        # Print metrics for this file
        if funcs:
            print("\nFile Totals:")
            print(sep.join([file.name(), str(file_metric['n1']), str(file_metric['n2']), str(file_metric['N1']), str(file_metric['N2']), str(file_metric['N']), str(file_metric['n']), str(file_metric['V']), str(file_metric['D']), str(file_metric['E']), '\n\n']))
        

            # Add the file_metric dictionary metrics to the project_metric dictionary
            project_metric['n1'] += file_metric['n1']
            project_metric['n2'] += file_metric['n2']
            project_metric['N1'] += file_metric['N1']
            project_metric['N2'] += file_metric['N2']
            project_metric['N'] += file_metric['N'] 
            project_metric['n'] += file_metric['n'] 
            project_metric['V'] += file_metric['V'] 
            project_metric['D'] += file_metric['D'] 
            project_metric['E'] += file_metric['E'] 

    # Print metrics for the project
    print("Project Totals:\n")
    print(sep.join([str(project_metric['n1']), str(project_metric['n2']), str(project_metric['N1']), str(project_metric['N2']), str(project_metric['N']), str(project_metric['n']), str(project_metric['V']), str(project_metric['D']), str(project_metric['E'])]))

