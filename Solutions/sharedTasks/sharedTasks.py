import functools
import understand

refKinds = 'Modify, Set, Use'
funKinds = 'Function ~Unknown ~Unresolved, Method ~Unknown ~Unresolved, Procedure ~Unknown ~Unresolved'
objKinds = 'Global Object'

taskFields = ['priority', 'core']

def refComparator(a, b):
    if a.line() < b.line() or a.line() == b.line() and a.column() < b.column():
        return -1
    if a.line() > b.line() or a.line() == b.line() and a.column() > b.column():
        return 1
    return 0
refComparator = functools.cmp_to_key(refComparator)


def refStr(ref):
  if ref.isforward():
    return f'{ref.scope().uniquename()} {ref.kind().longname()} {ref.ent().uniquename()} {ref.line()} {ref.column()}'
  return f'{ref.ent().uniquename()} {ref.kind().inv().longname()} {ref.scope().uniquename()} {ref.line()} {ref.column()}'

def getFunctionCallsOrGlobalObjectRefs(function, enableDisableFunctions):
    refs = function.refs('Call', 'Function')
    refs += function.refs(refKinds, objKinds)
    for ref in function.refs('use', 'macro'):
      if ref.ent() in enableDisableFunctions:
        refs.append(ref)
    refs.sort(key=refComparator)
    return refs


def checkControlledFunction(outerFunction, enableDisableFunctions, controlledFunctions, interruptDisabledRefs):
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
        elif ent.kind().check(objKinds):
            interruptDisabledRefs.add(refStr(ref))


def checkFunctionForInterruptControl(outerFunction, enableDisableFunctions, controlledFunctions, interruptDisabledRefs):
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

def getEdgeInfo(visited, tasks, incoming, outgoing, edgeInfo, root, fun, options):
    # Base case: visiting a function again from the same root
    funKey = str(root) + ' ' + str(fun)
    if funKey in visited:
        return
    visited.add(funKey)

    # References to global objects
    for ref in fun.refs(refKinds, objKinds):
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
    for call in fun.refs('Call', funKinds, True):
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


def filterIncomingEdges(incoming, outgoing, edgeInfo, ent, options):
    # Object
    if ent.kind().check(objKinds):
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

def parseArch(arch):
    tasks       = dict() # { ent: integer, ... }

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
            field = next((field for field in taskFields if field in name), "")
            foundFields.add(field)
            for fieldGroup in group.children():
                # Tasks
                for ent in fieldGroup.ents(True):
                    if not ent.kind().check(funKinds):
                        continue
                    if ent not in tasks:
                      tasks[ent] = dict()
                    tasks[ent][field] = fieldGroup.name()

            # Tasks
            for ent in group.ents(False):
                if ent in tasks or not ent.kind().check(funKinds):
                    continue
                tasks[ent] = dict()
    return (tasks, enableDisableFunctions, foundFields)

def buildEdgeInfo(db, arch, options):
    # Setup data for getEdgeInfo
    visited     = set()  # { funKey, ... }
    incoming    = dict() # { ent: set, ... }
    outgoing    = dict() # { ent: set, ... }
    edgeInfo    = dict() # { edgeKey: {
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
            if ent.kind().check(objKinds):
                filterIncomingEdges(incoming, outgoing, edgeInfo, ent, options)

    interruptDisabledRefs =  findInterruptDisabledRefs(db, enableDisableFunctions)

    return (edgeInfo, tasks, incoming, interruptDisabledRefs, foundFields)

def findInterruptDisabledRefs(db, enableDisableFunctions):
    # Setup data for checkFunctionForInterruptControl
    controlledFunctions    = set()  # { ent, ... }
    interruptDisabledRefs  = set()  # { str(ref), ... }

    # See which refs are interrupt-protected
    for fun in db.ents(funKinds):
        checkFunctionForInterruptControl(fun, enableDisableFunctions, controlledFunctions, interruptDisabledRefs)

    return interruptDisabledRefs


