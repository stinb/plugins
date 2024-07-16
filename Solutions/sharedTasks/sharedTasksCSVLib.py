# Library for shared tasks CSV scripts


import datetime
import functools
import os
import re

from understand import Arch, Db, Ent, IReport

from sharedTasks import *
from sharedTasksCSVLib import *


# Option keys
AUTO_EXPORT = 'autoExport'
CSV_FILE_LOCATION = 'csvFileLocation'
CSV_FILE_NAME = 'csvFileName'

# Default CSV export file name
DEFAULT_FILE_NAME = 'Shared_Tasks_for_{arch}_{date}_{time}.csv'

# Options for all shared tasks CSV plugins and scripts
CSV_OPTIONS = (
    Option(AUTO_EXPORT, 'Automatically export CSV file', OPTION_BOOL_CHOICES, OPTION_BOOL_FALSE),
    Option(CSV_FILE_LOCATION, 'CSV file location', [], os.getcwd()),
    Option(CSV_FILE_NAME, 'CSV file name', [], DEFAULT_FILE_NAME),
) + COMMON_OPTIONS


def entComparator(a: Ent, b: Ent) -> int:
    if a.name() < b.name():
        return -1
    if a.name() > b.name():
        return 1
    return 0
entComparator = functools.cmp_to_key(entComparator)


def printEnt(report: IReport, ent: Ent):
    report.entity(ent)
    report.print(ent.name())
    report.entity()


def printFile(report: IReport, file: Ent):
    report.entity(file)
    report.print(file.relname())
    report.entity()


def generateCSVRows(db: Db, arch: Arch, options: dict[str, str | bool], lines: list[str] | None, report: IReport | None):
    edgeInfo, tasks, incoming, interruptDisabledRefs, foundFields = buildEdgeInfo(db, arch, options)

    # Sort the objects
    objects = set()
    for edgeObj in edgeInfo.values():
        ent = edgeObj['ent']
        if not ent.kind().check(OBJ_ENT_KINDS):
            continue
        objects.add(ent)
    objects = list(objects)
    objects.sort(key=entComparator)

    # Sort the edge keys
    edgeKeysForObjects = dict()
    for obj in objects:
        edgeKeysForObjects[obj] = list(incoming[obj])
        edgeKeysForObjects[obj].sort()

    # header
    headerFields = [field for field in TASK_FIELDS if field in foundFields]

    header = 'Global Object, Shared, Protected, Function, Reference(M=Modify/S=Set/U=Use), File, Task'
    if headerFields:
      header += ', ' + ', '.join(field.capitalize() for field in headerFields)
    header += '\n'

    if report:
        report.print(header)
    if lines != None:
        lines.append(header)

    # Make rows for each object
    for obj in objects:

        # See how many tasks it is from
        fromTasks = set()
        fromTasksFiltered = set()
        edgeKeys = incoming[obj] if obj in incoming else set()
        for edgeKey in edgeKeys:
            info = edgeInfo[edgeKey]
            if not info['filtered'] and refStr(info['ref']) not in interruptDisabledRefs:
                fromTasksFiltered.update(info['from'])
            fromTasks.update(info['from'])
        shared = len(fromTasks) > 1
        if options[OBJECTS] == 'shared only' and not shared:
            continue

        # Info for each column for all rows of the object
        objectName = obj.name()
        shared = 'x' if len(fromTasksFiltered) > 1 else '-'

        # Make rows for each incoming edge to the object
        for edgeKey in edgeKeysForObjects[obj]:
            edgeObj = edgeInfo[edgeKey]

            # Info for each column for all rows of the edge
            protected = 'x' if refStr(edgeObj['ref']) in interruptDisabledRefs else '-'
            function = edgeObj['scope']
            functionName = edgeObj['scope'].name()
            reference = []
            for kindname in edgeObj['kindnames']:
                reference.append(kindname[0])
            reference.sort()
            reference = ''.join(reference)
            file = edgeObj['ref'].file()
            relname = file.relname()

            # Sort the tasks that this edge is from
            fromTasks = list(edgeObj['from'])
            fromTasks.sort(key=entComparator)

            # Make a single row for each task the edges are from
            for task in fromTasks:

                # Info for each column for this single row
                taskName = task.name()
                entFields = [tasks[task].get(field, '') for field in headerFields]

                # Make the clickable row for interactive report
                if report:
                    printEnt(report, obj)
                    report.print(f', {shared}, {protected}, ')
                    printEnt(report, function)
                    report.print(f', {reference}, ')
                    printFile(report, file)
                    report.print(', ')
                    printEnt(report, task)
                    if entFields:
                      report.print(', ' + ', '.join(entFields))
                    report.print('\n')

                # Make the row for the file
                if lines:
                    line = f'{objectName}, {shared}, {protected}, {functionName}, {reference}, {relname}, {taskName}'
                    if entFields:
                      line += ', ' +  ', '.join(entFields)
                    line += '\n'
                    lines.append(line)


def generateCSV(db: Db, arch: Arch, options: dict[str, str | bool], report: IReport | None = None):
    lines: list[str] | None = [] if options[AUTO_EXPORT] else None

    generateCSVRows(db, arch, options, lines, report)

    if not options[AUTO_EXPORT]:
        return

    # Default file name
    if not options[CSV_FILE_NAME]:
        options[CSV_FILE_NAME] = DEFAULT_FILE_NAME

    # Substitute {arch}
    if re.search('{arch}', options[CSV_FILE_NAME], flags=re.IGNORECASE):
        archSimplified = re.sub(r'\W|_', '', arch.name())
        options[CSV_FILE_NAME] = re.sub('{arch}', archSimplified, options[CSV_FILE_NAME], flags=re.IGNORECASE)

    # Substitute {date}
    now = None
    if re.search('{date}', options[CSV_FILE_NAME], flags=re.IGNORECASE):
        if not now:
            now = datetime.datetime.now()
        date = now.strftime('%Y_%m_%d')
        options[CSV_FILE_NAME] = re.sub('{date}', date, options[CSV_FILE_NAME], flags=re.IGNORECASE)

    # Substitute {time}
    if re.search('{time}', options[CSV_FILE_NAME], flags=re.IGNORECASE):
        if not now:
            now = datetime.datetime.now()
        time = now.strftime('%H_%M_%S')
        options[CSV_FILE_NAME] = re.sub('{time}', time, options[CSV_FILE_NAME], flags=re.IGNORECASE)

    # Append .csv to file name
    if not re.search(r'\.csv$', options[CSV_FILE_NAME]):
        options[CSV_FILE_NAME] += '.csv'

    # Export lines to a new file
    file = open(os.path.join(options[CSV_FILE_LOCATION], options[CSV_FILE_NAME]), 'w')
    file.writelines(lines)
    file.close()
