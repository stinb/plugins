import understand
import sys
import re

def file(ent):
    defRef = ent.ref("definein, declarin body")
    if defRef:
        return defRef.file


# Recurse through function's children while checking for purity
def checkPurity(func, seen):

    if func.uniquename() not in seen.keys():
        seen[func.uniquename()] = True
    if func.uniquename() in seen.keys() and seen[func.uniquename()] == False:
        return func, seen
    # Check if global or static is being read or written
    if func.refs("use, set, call, modify", "object global ~unresolved, object static ~unresolved"):
        impureCallsby(func, seen)
        return func, seen
    
    # Check if function is overridden by an impure function
    if func.refs("overriddenby"):
        override_refs = func.refs("overriddenby")
        for ref in override_refs:
            if ref.ent().uniquename() not in seen.keys():
                checkPurity(ref.ent(), seen)
                
            if seen[ref.ent().uniquename()] == False:
                impureCallsby(func, seen)
                return func, seen

    # Check for uses of unresoved function or objects
    if func.refs("use, set, call, modify", "object unresolved, function unresolved"):
        seen[func.uniquename()] = 'Uncertain'
        uncertainCallsby(func, seen)


    return func, seen

# Gather all functions and count whether they are pure, impure, or uncertain
def funcPurity():
    seen = {}
    pures = 0
    impures = 0
    uncertains = 0

    for func in db.ents("function,method,procedure"):
        if func.uniquename() not in seen.keys():
            seen[func.uniquename()] = True
        checkPurity(func, seen)

    for func in db.ents("function,method,procedure"):
        if seen[func.uniquename()] == True:
            pures += 1
            print(f'Pure: {func.longname()}')

        elif seen[func.uniquename()] == False:
            impures += 1

        elif seen[func.uniquename()] == 'Uncertain':
            uncertains += 1

    print(f'Pures:      {pures}\nImpures:    {impures}\nUncertains: {uncertains}')

def impureCallsby(func, seen):
    if func.uniquename() in seen.keys() and seen[func.uniquename()] == False:
        return func, seen

    seen[func.uniquename()] = False
    # Check children for purity
    children = func.refs("callby")
    if children:
        for child in children:
            child1, seen = impureCallsby(child.ent(), seen)
    return func, seen

def uncertainCallsby(func, seen):
    if func.uniquename() in seen.keys() and seen[func.uniquename()] == False:
        return func, seen

    seen[func.uniquename()] = 'Uncertain'
    # Check children for purity
    children = func.refs("callby")
    if children:
        for child in children:
            child1, seen = impureCallsby(child.ent(), seen)
    return func, seen


if __name__ == '__main__':
    # Open Database
    args = sys.argv
    db = understand.open(args[1])

    funcPurity()
