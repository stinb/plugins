# Library for all shared tasks plugins and scripts


import dataclasses
import functools
import re
from dataclasses import dataclass
from enum import Enum

import understand
from understand import Arch, Db, Ent, Ref


class DictResult(Enum):
    NotInDict = 0
    NotInDb = 1


# Make calls to the database, but with a cache
class DbCache:
    def __init__(self):
        self.entEntsCache: dict[str, list[Ent]] = dict()
        self.entRefCache: dict[str, Ref | DictResult] = dict()

    # Call ent.ents
    def entEnts(self, ent: Ent, refKind: str | None = None, entKind: str | None = None) -> list[Ent]:
        key: str = f'{ent.id()} "{refKind}" "{entKind}"'
        value: list[Ent] | None = self.entEntsCache.get(key)
        if value == None:
            value = ent.ents(refKind, entKind)
            self.entEntsCache[key] = value
            return value
        return value

    # Call ent.ref
    def entRef(self, ent: Ent, refKind: str | None = None, entKind: str | None = None) -> Ref | None:
        key: str = f'{ent.id()} "{refKind}" "{entKind}"'
        value: Ref | DictResult = self.entRefCache.get(key, DictResult.NotInDict)
        if value == DictResult.NotInDict:
            value = ent.ref(refKind, entKind)
            if value == None:
                self.entRefCache[key] = DictResult.NotInDb
                return None
            self.entRefCache[key] = value
            return value
        elif value == DictResult.NotInDb:
            return None
        return value


@dataclass
class InventoryItem:
    name: str
    unit_price: float
    quantity_on_hand: int = 0


@dataclass
class Option:
    # Used in the CLI and in these scripts
    key: str
    # Used in graph options
    name: str
    choices: list[str]
    default: str


# Ref kinds and ent kinds
FUN_REF_KINDS = 'Call, Assign FunctionPtr'
FUN_REF_KINDS_OPTION_PTR = ', Use Ptr'
FUN_REF_KINDS_OPTION_OVERRIDE = ', Overriddenby'
FUN_REF_KINDS_OPTION_INSTANCEOF = ', Instanceof'
OBJ_ENT_KINDS = 'Object'
OBJ_REF_KINDS = 'Modify, Set, Use'
OBJ_REF_KINDS_OPTION = ', Deref Call'

# Patterns to look for in sub-architectures
TASK_FIELDS = ('priority', 'core')

# Option keys
DEPTH = 'depth'
FILTER_MODIFY_SET_ONLY = 'filterModifySetOnly'
FILTER_USE_ONLY = 'filterUseOnly'
FUNCTION_INSTANCES = 'functionInstances'
MEMBER_FUNCTIONS = 'memberFunctions'
MEMBER_OBJECT_PARENTS = 'memberObjectParents'
MEMBER_OBJECTS = 'memberObjects'
OBJECTS = 'objects'
OVERRIDES = 'overrides'
REFERENCE = 'reference'
FUNCTION_POINTER = 'functionPointer'

# Option values and choices of values
OPTION_BOOL_TRUE = 'On'
OPTION_BOOL_FALSE = 'Off'
OPTION_BOOL_CHOICES = [OPTION_BOOL_TRUE, OPTION_BOOL_FALSE]
OPTION_DEPTH_CHOICES = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', 'All']

# Options used in all shared tasks plugins and scripts
COMMON_OPTIONS = (
    Option(DEPTH, 'Depth', OPTION_DEPTH_CHOICES, 'All'),
    Option(FILTER_MODIFY_SET_ONLY, 'Filter out modify/set only', OPTION_BOOL_CHOICES, OPTION_BOOL_FALSE),
    Option(FILTER_USE_ONLY, 'Filter out use only', OPTION_BOOL_CHOICES, OPTION_BOOL_FALSE),
    Option(FUNCTION_INSTANCES, 'Instance references', OPTION_BOOL_CHOICES, OPTION_BOOL_FALSE),
    Option(FUNCTION_POINTER, 'Function pointers', OPTION_BOOL_CHOICES, OPTION_BOOL_FALSE),
    Option(MEMBER_FUNCTIONS, 'Member functions', ['Longer name', 'Long name', 'Name'], 'Long name'),
    Option(MEMBER_OBJECT_PARENTS, 'Member object parents', OPTION_BOOL_CHOICES, OPTION_BOOL_TRUE),
    Option(MEMBER_OBJECTS, 'Member objects', ['Long name', 'Name', 'Off'], 'Long name'),
    Option(OBJECTS, 'Objects', ['All', 'Shared only'], 'All'),
    Option(OVERRIDES, 'Overrides', OPTION_BOOL_CHOICES, OPTION_BOOL_FALSE),
    Option(REFERENCE, 'Reference', ['All', 'Simple'], 'All'),
)


def checkIsCallable(ent: Ent) -> bool:
    if ent.kind().check('Function, Procedure, Subroutine, Method'):
        return True
    if ent.ref('Callby, Assignby FunctionPtr'):
        return True
    return False


def refComparator(a: Ref, b: Ref) -> int:
    aLine = a.line()
    bLine = b.line()
    if aLine < bLine:
        return -1
    if aLine > bLine:
        return 1
    aColumn = a.column()
    bColumn = b.column()
    if aColumn < bColumn:
        return -1
    if aColumn > bColumn:
        return 1
    return 0
refComparator = functools.cmp_to_key(refComparator)


def refStr(ref: Ref) -> str:
    if ref.isforward():
        return f'{ref.scope().id()} {ref.kind().longname()} {ref.ent().id()} {ref.line()} {ref.column()}'
    return f'{ref.ent().id()} {ref.kind().inv().longname()} {ref.scope().id()} {ref.line()} {ref.column()}'


def getLongName(ent: Ent, options: dict[str, str | bool]) -> str:
    result = ent.name()

    # Member function
    entKind = ent.kind()
    if entKind.check('Member Function'):
        if MEMBER_FUNCTIONS not in options:
            return ent.name()
        optionValue = options[MEMBER_FUNCTIONS]
        if optionValue == 'Longer name':
            return getLongerName(ent, ent.parent())
        elif optionValue == 'Long name':
            return ent.longname()
        else:
            return ent.name()
    # Member object parent
    elif entKind.check('Member Object'):
        if MEMBER_OBJECTS not in options:
            return ent.name()
        optionValue = options[MEMBER_OBJECTS]
        if optionValue == 'Long name':
            return ent.longname()

    return result


def getLongerName(ent: Ent, parentEnt: Ent) -> str:
    longname = ent.longname()

    parent = ent.parent()
    if not parent:
        return longname

    parentTypes = parent.ents('Typed')
    if len(parentTypes) != 1:
        return longname

    return f'{parentTypes[0].name()}::{longname}'


def getFnRefKinds(options: dict[str, str | bool] | None = None) -> str:
    refKinds = FUN_REF_KINDS
    if not options:
        return refKinds
    if options[FUNCTION_POINTER]:
        refKinds += FUN_REF_KINDS_OPTION_PTR
    if options[OVERRIDES]:
        refKinds += FUN_REF_KINDS_OPTION_OVERRIDE
    if options[FUNCTION_INSTANCES]:
        refKinds += FUN_REF_KINDS_OPTION_INSTANCEOF
    return refKinds


def getFnOrObjRefs(
        cache: DbCache,
        function: Ent,
        enableDisableFunctions: dict,
        options: dict[str, str | bool] | None = None) -> list[Ref]:

    refKinds = getFnRefKinds(options)

    refs = function.refs(refKinds)
    refs += globalObjRefs(cache, function, options)
    for ref in function.refs('Use', 'Macro'):
        if ref.ent() in enableDisableFunctions:
            refs.append(ref)
    refs.sort(key=refComparator)
    return refs


def entHasMembers(ent: Ent) -> bool:
    for entType in ent.ents('Typed'):
        if entType.ref('Define', 'Member Object'):
            return True
    return False


# Recursively see if this entity is a (unique instance of a) member object of a
# global object
def isMemberOfGlobal(cache: DbCache, ent: Ent) -> bool:
    return isGeneralMemberOfGlobal(cache, ent) or isUniqueMemberOfGlobal(cache, ent)


# Recursively see if this entity is a member object of a global object
def isGeneralMemberOfGlobal(cache: DbCache, ent: Ent) -> bool:
    parent = ent.parent()
    if not parent:
        return False

    parentKind = parent.kind()
    if parentKind.check('Global Object'):
        return True
    elif parentKind.check('Member Object') and isMemberOfGlobal(cache, parent):
        return True

    return False


# Recursively see if this entity is a unique instance of a member object of a
# global object
def isUniqueMemberOfGlobal(cache: DbCache, ent: Ent) -> bool:
    parentType = ent.parent()
    if not parentType:
        return False

    if cache.entRef(parentType, 'Typedby', 'Object'):
        return True

    for parentInstance in cache.entEnts(parentType, 'Typedby', 'Member Object'):
        if isMemberOfGlobal(cache, parentInstance):
            return True

    return False


def globalObjRefs(cache: DbCache, function: Ent, options: dict[str, str | bool] | None = None) -> list[Ref]:
    result = []

    # Get options
    memberObjectParents = not options or MEMBER_OBJECT_PARENTS not in options \
        or options[MEMBER_OBJECT_PARENTS]
    memberObjects = not options or MEMBER_OBJECTS not in options \
        or options[MEMBER_OBJECTS] != 'Off'

    functions = [function]
    if not options or not options[FUNCTION_INSTANCES]:
         functions += function.ents('Instanceof')

    for otherFunction in functions:
        # Global objects
        refKinds = OBJ_REF_KINDS
        if options and options[FUNCTION_POINTER] == 'On':
            refKinds += OBJ_REF_KINDS_OPTION

        for ref in otherFunction.refs(refKinds, 'Global Object'):
            if not memberObjectParents and entHasMembers(ref.ent()):
                continue
            result.append(ref)
        # Members of global objects
        if memberObjects:
            for ref in otherFunction.refs(refKinds, 'Member Object'):
                if not isMemberOfGlobal(cache, ref.ent()):
                    continue
                result.append(ref)

    result.sort(key=refComparator)
    return result


def checkControlledFunction(
        cache: DbCache,
        outerFunction: Ent,
        enableDisableFunctions: dict,
        controlledFunctions: set[Ent],
        interruptDisabledRefs: set[str],
        options: dict[str, str | bool] | None = None):

    # Base case: already checked
    if outerFunction in controlledFunctions:
        return
    controlledFunctions.add(outerFunction)

    # Function call or global object modify/set/use
    for ref in getFnOrObjRefs(cache, outerFunction, enableDisableFunctions, options):
        ent = ref.ent()

        # Recurse for each function called, ignoring enable/disable functions
        if ent.kind().check('Function'):
            if ent not in enableDisableFunctions and ent not in controlledFunctions:
                interruptDisabledRefs.add(refStr(ref))
                checkControlledFunction(cache, ent, enableDisableFunctions, controlledFunctions, interruptDisabledRefs, options)

        # Global object ref
        elif ent.kind().check(OBJ_ENT_KINDS):
            interruptDisabledRefs.add(refStr(ref))


def checkFunctionForInterruptControl(
        cache: DbCache,
        outerFunction: Ent,
        enableDisableFunctions: dict,
        controlledFunctions: set[Ent],
        interruptDisabledRefs: set[str],
        options: dict[str, str | bool] | None = None):

    # Disable functions that are making the code interrupt-protected
    interruptDisabledFunctions = set() # { ent, ...  }

    # Function call or global object modify/set/use
    for ref in getFnOrObjRefs(cache, outerFunction, enableDisableFunctions, options):
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
            checkControlledFunction(cache, ent, enableDisableFunctions, controlledFunctions, interruptDisabledRefs, options)

        # Global object ref with interrupt disabled
        elif not ent.kind().check('Function') and len(interruptDisabledFunctions):
            interruptDisabledRefs.add(refStr(ref))


def getEdgeInfo(
        cache: DbCache,
        visited: set[str],
        tasks: dict,
        incoming: dict,
        outgoing: dict,
        edgeInfo: dict,
        root: Ent,
        fun: Ent,
        options: dict[str, str | bool],
        depth: int):

    # Base case: visiting a function again from the same root
    funKey = f'{root.id()} {fun.id()}'
    if funKey in visited:
        return
    visited.add(funKey)

    # Base case: max depth
    maxDepth = options[DEPTH]
    if maxDepth != 'All' and depth >= maxDepth:
        return

    # References to global objects
    for ref in globalObjRefs(cache, fun, options):
        scope = root if options[REFERENCE] == 'Simple' else fun
        ent = ref.ent()

        edgeKey = f'{scope.id()} {ent.id()}'

        kindname = ref.kind().longname()
        if 'Use' in kindname:
            kindname = 'Use'
        elif 'Set' in kindname:
            kindname = 'Set'
        elif 'Modify' in kindname:
            kindname = 'Modify'
        elif 'Call' in kindname:
            kindname = 'Call'

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
    refKinds = getFnRefKinds(options)

    for call in fun.refs(refKinds, '~Unknown', True):
        if options[REFERENCE] == 'All':
            scope = call.scope()
            ent = call.ent()

            edgeKey = f'{scope.id()} {ent.id()}'

            # Add to edge info
            if edgeKey not in edgeInfo:
                edgeInfo[edgeKey] = {
                    'root': scope in tasks,
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

            # Add assignby ref to edge
            for assby_ref in call.ent().refs("Assignby Functionptr"):
                getEdgeInfo(cache, visited, tasks, incoming, outgoing,
                            edgeInfo, root, assby_ref.ent(), options, depth + 1)

        getEdgeInfo(cache, visited, tasks, incoming, outgoing, edgeInfo, root, call.ent(), options, depth + 1)


def filterIncomingEdges(
        incoming: dict,
        outgoing: dict,
        edgeInfo: dict,
        ent: Ent,
        options: dict[str, str | bool]):

    # Object
    if ent.kind().check(OBJ_ENT_KINDS):
        # Get all kindnames for incoming edges
        allEdgeKindnames = set()
        for edgeKey in incoming[ent]:
            edgeObj = edgeInfo[edgeKey]
            allEdgeKindnames.update(edgeObj['kindnames'])

        # See if the object should be filtered out
        filtered = False
        if options[FILTER_USE_ONLY] and 'Set' not in allEdgeKindnames and 'Modify' not in allEdgeKindnames \
        or options[FILTER_MODIFY_SET_ONLY] and 'Use' not in allEdgeKindnames:
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
        elif 'task' in name or any(field in name for field in TASK_FIELDS):
            # Priority/Core groups
            field = next((field for field in TASK_FIELDS if field in name), '')
            foundFields.add(field)
            for fieldGroup in group.children():
                # Tasks
                for ent in fieldGroup.ents(True):
                    if not checkIsCallable(ent):
                        continue
                    if ent not in tasks:
                        tasks[ent] = dict()
                    tasks[ent][field] = fieldGroup.name()

            # Tasks
            for ent in group.ents(False):
                if ent in tasks or not checkIsCallable(ent):
                    continue
                tasks[ent] = dict()
    return (tasks, enableDisableFunctions, foundFields)


def buildEdgeInfo(
        db: Db,
        arch: Arch,
        options: dict[str, str | bool]) -> (dict, dict, set[str], set[str], set[str]):

    # Setup data for getEdgeInfo
    cache: DbCache = DbCache()
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
        getEdgeInfo(cache, visited, tasks, incoming, outgoing, edgeInfo, ent, ent, options, 0)

    # See which edges are filtered out
    if options[FILTER_MODIFY_SET_ONLY] or options[FILTER_USE_ONLY]:
        for edgeObj in edgeInfo.values():
            ent = edgeObj['ent']
            if ent.kind().check(OBJ_ENT_KINDS):
                filterIncomingEdges(incoming, outgoing, edgeInfo, ent, options)

    interruptDisabledRefs = findInterruptDisabledRefs(db, cache, enableDisableFunctions, options)

    # Decide whether the nodes are shared
    sharedObjects: dict[Ent, bool] = dict()
    fromRootsFiltered = set()
    for edgeObj in edgeInfo.values():
        ent = edgeObj['ent']
        if ent not in sharedObjects:
            # See how many root functions each node it's from
            fromRootsFiltered.clear()
            if ent.kind().check(OBJ_ENT_KINDS):
                edgeKeys = incoming[ent] if ent in incoming else set()
                for edgeKey in edgeKeys:
                    info = edgeInfo[edgeKey]
                    if not info['filtered'] and refStr(info['ref']) not in interruptDisabledRefs:
                        fromRootsFiltered.update(info['from'])
            sharedObjects[ent] = len(fromRootsFiltered) > 1
        # Remember that it's shared
        edgeObj['shared'] = sharedObjects[ent]

    # If shared only, then delete edges to non-shared objects
    if options[OBJECTS] == 'Shared only':
        for edgeKey, edgeObj in edgeInfo.copy().items():
            scope = edgeObj['scope']
            ent = edgeObj['ent']
            # Skip if not at the end
            if ent in outgoing:
                continue
            # Skip if not a shared object
            if edgeObj['shared'] or not ent.kind().check(OBJ_ENT_KINDS):
                continue
            # Delete incoming edges leading to a dead end
            edgeKeysToDeleteStack: list[str] = [edgeKey]
            while edgeKeysToDeleteStack:
                # Pop the edge from the stack
                edgeKey = edgeKeysToDeleteStack.pop()
                scope = edgeInfo[edgeKey]['scope']
                ent = edgeInfo[edgeKey]['ent']
                # If there are no other outgoing edges (siblings)
                if len(outgoing[scope]) == 1:
                    # Push the incoming edges to the stack
                    for otherEdgeKey in incoming[scope]:
                        edgeKeysToDeleteStack.append(otherEdgeKey)
                # Delete the edge
                outgoing[scope].remove(edgeKey)
                incoming[ent].remove(edgeKey)
                del edgeInfo[edgeKey]

    return (edgeInfo, tasks, incoming, interruptDisabledRefs, foundFields)


def findInterruptDisabledRefs(
        db: Db,
        cache: DbCache,
        enableDisableFunctions: dict,
        options: dict[str, str | bool] | None = None) -> set[str]:

    # Setup data for checkFunctionForInterruptControl
    controlledFunctions = set()   # { ent, ... }
    interruptDisabledRefs = set() # { str(ref), ... }

    # See which refs are interrupt-protected
    for fun in db.ents():
        if not checkIsCallable(fun):
            continue
        checkFunctionForInterruptControl(cache, fun, enableDisableFunctions, controlledFunctions, interruptDisabledRefs, options)

    return interruptDisabledRefs


def checkIsFunctionPointer(ent: Ent) -> bool:
    if not ent.kind().check('Parameter, Object'):
        return False
    t = ent.freetext('UnderlyingType')
    t = re.sub(r'\b(const|restrict|volatile)\s*', '', t)
    return bool(re.search(r'\(\*+\)\(', t))
