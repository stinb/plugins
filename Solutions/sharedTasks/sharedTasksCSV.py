# Shared library for the Shared Tasks CSV interactive report: one table row per
# task/function/object reference. CSV output is produced by the report
# framework's export (und report -format csv / GUI), not by this script.


import functools

from understand import Arch, Db, Ent, ReportContext
from sharedTasks import *


def entComparator(a: Ent, b: Ent) -> int:
    if a.name() < b.name():
        return -1
    if a.name() > b.name():
        return 1
    return 0
entComparator = functools.cmp_to_key(entComparator)


# Emit one table cell. The report renders a real report.table(), so the
# framework's CSV export builds and quotes the columns.
def cellEnt(report: ReportContext, ent: Ent, options: dict[str, str | bool]):
    report.tablecell()
    report.entity(ent)
    report.print(getLongName(ent, options))
    report.entity()


def cellFile(report: ReportContext, file: Ent):
    report.tablecell()
    report.entity(file)
    report.print(file.relname())
    report.entity()


def cellText(report: ReportContext, text: str):
    report.tablecell()
    report.print(text)


def generateCSV(db: Db, arch: Arch, options: dict[str, str | bool], report: ReportContext):
    edgeInfo, tasks, incoming, interruptDisabledRefs, foundFields, arrayMemberEdges = buildEdgeInfo(db, arch, options)

    # Sort the objects
    objects = set()
    for edgeObj in edgeInfo.values():
        ent = edgeObj['ent']
        if not ent.kind().check(OBJ_ENT_KINDS):
            continue
        objects.add(ent)
    # With the pointers-to-globals option, sort by display label so a pointer's
    # members group together (e.g. ptr(gDat.ST2).mem_C next to
    # ptr(gDat.ST2).mem_D); otherwise keep the legacy short-name ordering.
    objects = list(objects)
    if options.get(POINTERS_TO_GLOBALS):
        objects.sort(key=lambda obj: getLongName(obj, options))
    else:
        objects.sort(key=entComparator)

    # Sort the edge keys
    edgeKeysForObjects = dict()
    for obj in objects:
        edgeKeysForObjects[obj] = list(incoming[obj])
        edgeKeysForObjects[obj].sort()

    # Table columns
    headerFields = [field for field in TASK_FIELDS if field in foundFields]

    referenceString = "Reference(M=Modify/S=Set/U=Use)"
    if options[FUNCTION_POINTER] == 'On':
        referenceString = "Reference(M=Modify/S=Set/U=Use/C=Call)"
    columns = ['Global Object', 'Shared', 'Protected', 'Function', referenceString, 'File', 'Task']
    columns += [field.capitalize() for field in headerFields]

    report.table(columns)

    # Make rows for each object
    for obj in objects:

        # Match the graph's lockset-based classification (computed in
        # buildEdgeInfo via classify_shared_objects). Shared = reached from
        # multiple task roots without a consistent lock. Protected = reached
        # from multiple task roots with a consistent lock. Single-task
        # objects are neither.
        edgeKeys = incoming[obj] if obj in incoming else set()
        fromTasksFiltered: set[Ent] = set()
        isShared = False
        for edgeKey in edgeKeys:
            info = edgeInfo[edgeKey]
            if not info['filtered']:
                fromTasksFiltered.update(info['from'])
            if info['shared']:
                isShared = True

        if options[OBJECTS] == 'shared only' and not isShared:
            continue

        # Info for each column for all rows of the object
        shared = 'x' if isShared else '-'
        protected = 'x' if (len(fromTasksFiltered) > 1 and not isShared) else '-'

        # Make rows for each incoming edge to the object
        for edgeKey in edgeKeysForObjects[obj]:
            edgeObj = edgeInfo[edgeKey]

            # Info for each column for all rows of the edge
            function = edgeObj['scope']
            reference = []
            for kindname in edgeObj['kindnames']:
                reference.append(kindname[0])
            reference.sort()
            reference = ''.join(reference)
            file = edgeObj['ref'].file()

            # Make a single row for each task the edges are from
            for task in sorted(edgeObj['from'], key=entComparator):
                entFields = [tasks[task].get(field, '') for field in headerFields]

                cellEnt(report, obj, options)
                cellText(report, shared)
                cellText(report, protected)
                cellEnt(report, function, options)
                cellText(report, reference)
                cellFile(report, file)
                cellEnt(report, task, options)
                for f in entFields:
                  cellText(report, f)

    # Array element member rows (informational, e.g. gDat.ST2[].mem_C). These are
    # synthesized (array, member) accesses with no shared/protected status.
    for edge in sorted(arrayMemberEdges, key=lambda e: (e['displayName'], e['scope'].name())):
        reference = ''.join(sorted(k[0] for k in edge['kindnames']))
        file = edge['ref'].file()

        for task in sorted(edge['from'], key=entComparator):
            entFields = [tasks[task].get(field, '') for field in headerFields]

            cellText(report, edge['displayName'])
            cellText(report, '-')
            cellText(report, '-')
            cellEnt(report, edge['scope'], options)
            cellText(report, reference)
            cellFile(report, file)
            cellEnt(report, task, options)
            for f in entFields:
              cellText(report, f)

    report.table()
