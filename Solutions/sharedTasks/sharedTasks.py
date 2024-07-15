import functools

import understand
from understand import Arch, Db, Ent, Ref


funRefKinds = 'Call, Use Ptr'
funEntKinds = 'Function ~Unknown ~Unresolved, Method ~Unknown ~Unresolved, Procedure ~Unknown ~Unresolved'
objEntKinds = 'Object'
objRefKinds = 'Modify, Set, Use'

taskFields = ['priority', 'core']


def refComparator(a: Ref, b: Ref) -> int:
    if a.line() < b.line() or a.line() == b.line() and a.column() < b.column():
        return -1
    if a.line() > b.line() or a.line() == b.line() and a.column() > b.column():
        return 1
    return 0
refComparator = functools.cmp_to_key(refComparator)


def refStr(ref: Ref) -> str:
    if ref.isforward():
        return f'{ref.scope().uniquename()} {ref.kind().longname()} {ref.ent().uniquename()} {ref.line()} {ref.column()}'
    return f'{ref.ent().uniquename()} {ref.kind().inv().longname()} {ref.scope().uniquename()} {ref.line()} {ref.column()}'


def getFunctionCallsOrGlobalObjectRefs(function: Ent, enableDisableFunctions: dict) -> list[Ref]:
    refs = function.refs(funRefKinds, 'Function')
    refs += globalObjRefs(function)
    for ref in function.refs('Use', 'Macro'):
        if ref.ent() in enableDisableFunctions:
            refs.append(ref)
    refs.sort(key=refComparator)
    return refs


def globalObjRefs(function: Ent) -> list[Ref]:
    refs = []
    objRefs = function.refs(objRefKinds, objEntKinds)
    objRefs.sort(key=refComparator)
    i = len(objRefs) - 1
    while i >= 0:
        ref = objRefs[i]
        if ref.ent().kind().check('Global'):
            # Keep references to global objects, like before
            refs.append(ref)

        elif (ref.ent().refs('C Instanceof') and
            ref.ent().parent() and
            ref.ent().parent().kind().check('Global')):
        # If member object settings are on, then members of global structs will
        # have associated entities with an "instanceof" reference. So, if this
        # object has an instanceof reference to a global object parent, keep
        # the reference
        refs.append(ref)

        if i > 0 and objRefs[i-1].kind() == ref.kind() and objRefs[i-1].ent() == ref.ent().parent():
            # Both the object and the member get the reference so skip the
            # reference to the parent object if it exists
            i -= 1
        i -= 1
    return refs


def checkControlledFunction(outerFunction: Ent, enableDisableFunctions: dict, controlledFunctions: set[Ent], interruptDisabledRefs: set[str]):
    # Base case: already checked
    if outerFunction in controlledFunctions:
        return
    controlledFunctions.add(outerFunction)

    # Function call or global object modify/set/use
    for ref in getFunctionCallsOrGlobalObjectRefs(outerFunction, enableDisableFunctions):
        ent = ref.ent()

        # Recurse for each function called, ignoring enable/disable functions
        if ent.kind().check('Function'):
            if ent not in enableDisableFunctions and ent not in controlledFunctions:
                interruptDisabledRefs.add(refStr(ref))
                checkControlledFunction(ent, enableDisableFunctions, controlledFunctions, interruptDisabledRefs)

        # Global object ref
        elif ent.kind().check(objEntKinds):
            interruptDisabledRefs.add(refStr(ref))


def checkFunctionForInterruptControl(outerFunction: Ent, enableDisableFunctions: dict, controlledFunctions: set[Ent], interruptDisabledRefs: set[str]):
    # Disable functions that are making the code interrupt-protected
    interruptDisabledFunctions = set() # { ent, ...  }

    # Function call or global object modify/set/use
    for ref in getFunctionCallsOrGlobalObjectRefs(outerFunction, enableDisableFunctions):
        ent = ref.ent()

        # Add/remove the disable outerFunction
        if ent in enableDisableFunctions:
            if enableDisableFunctions[ent]['disable']:
                disable = ent
                interruptDisabledFunctions.add(ent)
            else:
                disable = enableDisableFunctions[ent]['other']
                interruptDisabledFunctions.discard(disable)

        # Recurse for each function called in an interrupt-protected area,
        # ignoring enable/disable functions
        elif ent.kind().check('Function') and len(interruptDisabledFunctions) and ent not in controlledFunctions:
            interruptDisabledRefs.add(refStr(ref))
            checkControlledFunction(ent, enableDisableFunctions, controlledFunctions, interruptDisabledRefs)

        # Global object ref with interrupt disabled
        elif not ent.kind().check('Function') and len(interruptDisabledFunctions):
            interruptDisabledRefs.add(refStr(ref))


def getEdgeInfo(visited: set[str], tasks: dict, incoming: dict, outgoing: dict, edgeInfo: dict, root: Ent, fun: Ent, options: dict):
    # Base case: visiting a function again from the same root
    funKey = str(root) + ' ' + str(fun)
    if funKey in visited:
        return
    visited.add(funKey)

    # References to global objects
    for ref in globalObjRefs(fun):
        scope = root if options['reference'] == 'Simple' else fun
        ent = ref.ent()

        edgeKey = str(scope) + ' ' + str(ent)

        kindname = ref.kindname().split(' ').pop()

        # Add to edge info
        if edgeKey not in edgeInfo:
            edgeInfo[edgeKey] = {
                'root': scope in tasks,
                'scope': scope,
                'ent': ent,
                'kindnames': set(),
                'from': set(),
                'filtered': False,
                'ref': ref,
            }
        edgeObj = edgeInfo[edgeKey]
        edgeObj['kindnames'].add(kindname)
        edgeObj['from'].add(root)

        # Add to incoming node edges
        if ent not in incoming:
            incoming[ent] = set()
        incoming[ent].add(edgeKey)

        # Add to outgoing node edges
        if scope not in outgoing:
            outgoing[scope] = set()
        outgoing[scope].add(edgeKey)

    # Function calls
    for call in fun.refs(funRefKinds, funEntKinds, True):
        if options['reference'] == 'All':
            scope = call.scope()
            ent = call.ent()

            edgeKey = str(scope) + ' ' + str(ent)

            # Add to edge info
            if edgeKey not in edgeInfo:
                edgeInfo[edgeKey] = {
                    'root': ent in tasks,
                    'scope': scope,
                    'ent': ent,
                    'kindnames': set(),
                    'from': set(),
                    'filtered': False,
                    'ref': call,
                }
            edgeInfo[edgeKey]['from'].add(root)

            # Add to incoming node edges
            if ent not in incoming:
                incoming[ent] = set()
            incoming[ent].add(edgeKey)

            # Add to outgoing node edges
            if scope not in outgoing:
                outgoing[scope] = set()
            outgoing[scope].add(edgeKey)

        getEdgeInfo(visited, tasks, incoming, outgoing, edgeInfo, root, call.ent(), options)


def filterIncomingEdges(incoming: dict, outgoing: dict, edgeInfo: dict, ent: Ent, options: dict):
    # Object
    if ent.kind().check(objEntKinds):
        # Get all kindnames for incoming edges
        allEdgeKindnames = set()
        for edgeKey in incoming[ent]:
            edgeObj = edgeInfo[edgeKey]
            allEdgeKindnames.update(edgeObj['kindnames'])

        # See if the object should be filtered out
        filtered = False
        if options['filterUseOnly'] and 'Set' not in allEdgeKindnames and 'Modify' not in allEdgeKindnames \
        or options['filterModifySetOnly'] and 'Use' not in allEdgeKindnames:
            filtered = True

    # Function
    else:
        # Get all outgoing edges
        filtered = True
        for edgeKey in outgoing[ent]:
            edgeObj = edgeInfo[edgeKey]
            if not edgeObj['filtered']:
                filtered = False
                break

    # Base case: stop filtering out edges
    if not filtered or ent not in incoming:
        return

    # Filter out all incoming edges
    for edgeKey in incoming[ent]:
        edgeObj = edgeInfo[edgeKey]
        edgeObj['filtered'] = True

    # Recurse for all scope nodes of incoming edges
    for edgeKey in incoming[ent]:
        edgeObj = edgeInfo[edgeKey]
        scope = edgeObj['scope']
        filterIncomingEdges(incoming, outgoing, edgeInfo, scope, options)


def parseArch(arch: Arch) -> (dict, dict, set[str]):
    tasks = dict() # { ent: integer, ... }
    enableDisableFunctions = dict() # { ent: { 'disable': boolean, 'other': ent }, ... }
    foundFields = set()

    # Parse the architecture
    for group in arch.children():
        name = group.name().lower()
        # ARCH_NAME/.*interrupt.*/GROUP_NAME/.*disable.*/FUNCTION_NAME
        # ARCH_NAME/.*interrupt.*/GROUP_NAME/.*enable.*/FUNCTION_NAME
        if 'interrupt' in name or 'control' in name:
            for interruptPair in group.children():
                disableAndEnableGroups = interruptPair.children()
                if len(disableAndEnableGroups) != 2:
                    continue
                disable = None
                enable = None
                for disableOrEnableGroup in disableAndEnableGroups:
                    name = disableOrEnableGroup.name().lower()
                    ents = disableOrEnableGroup.ents(False)
                    if len(ents) != 1:
                        continue
                    ent = ents[0]
                    if not ent.kind().check('Function, Macro'):
                        continue
                    if 'disable' in name:
                        disable = ent
                    elif 'enable' in name:
                        enable = ent
                if not disable or not enable:
                    continue
                enableDisableFunctions[disable] = {'disable': True, 'other': enable}
                enableDisableFunctions[enable] = {'disable': False, 'other': disable}

        # ARCH_NAME/.*priority.*/INTEGER/FUNCTION_NAME
        elif 'task' in name or any(field in name for field in taskFields):
            # Priority/Core groups
            field = next((field for field in taskFields if field in name), '')
            foundFields.add(field)
            for fieldGroup in group.children():
                # Tasks
                for ent in fieldGroup.ents(True):
                    if not ent.kind().check(funEntKinds):
                        continue
                    if ent not in tasks:
                      tasks[ent] = dict()
                    tasks[ent][field] = fieldGroup.name()

            # Tasks
            for ent in group.ents(False):
                if ent in tasks or not ent.kind().check(funEntKinds):
                    continue
                tasks[ent] = dict()
    return (tasks, enableDisableFunctions, foundFields)


def buildEdgeInfo(db: Db, arch: Arch, options: dict) -> (dict, dict, set[str], set[str], set[str]):
    # Setup data for getEdgeInfo
    visited = set()   # { funKey, ... }
    incoming = dict() # { ent: set, ... }
    outgoing = dict() # { ent: set, ... }
    edgeInfo = dict() # { edgeKey: {
                      #     'root': boolean,
                      #     'scope': ent,
                      #     'ent': ent,
                      #     'kindnames': set,
                      #     'from': set,
                      #     'filtered': boolean,
                      #     'ref': ref
                      # }, ... }
    tasks, enableDisableFunctions, foundFields = parseArch(arch)

    # Get the refs going to each object/function
    for ent in tasks.keys():
        getEdgeInfo(visited, tasks, incoming, outgoing, edgeInfo, ent, ent, options)

    # See which edges are filtered out
    if options['filterModifySetOnly'] or options['filterUseOnly']:
        for edgeObj in edgeInfo.values():
            ent = edgeObj['ent']
            if ent.kind().check(objEntKinds):
                filterIncomingEdges(incoming, outgoing, edgeInfo, ent, options)

    interruptDisabledRefs = findInterruptDisabledRefs(db, enableDisableFunctions)

    return (edgeInfo, tasks, incoming, interruptDisabledRefs, foundFields)


def findInterruptDisabledRefs(db: Db, enableDisableFunctions: dict) -> set[str]:
    # Setup data for checkFunctionForInterruptControl
    controlledFunctions = set()   # { ent, ... }
    interruptDisabledRefs = set() # { str(ref), ... }

    # See which refs are interrupt-protected
    for fun in db.ents(funEntKinds):
        checkFunctionForInterruptControl(fun, enableDisableFunctions, controlledFunctions, interruptDisabledRefs)

    return interruptDisabledRefs
