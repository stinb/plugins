#! /usr/bin/env upython


# CLI for creating an interactive report of an architecture of shared tasks and
# optionally exporting it as a CSV


import datetime
import functools
import os
import re
import sys

import understand
from understand import Arch, Db, Ent, IReport

from sharedTasks import *


#######
# All #
#######


# Option keys
AUTO_EXPORT = 'autoExport'
CSV_FILE_LOCATION = 'csvFileLocation'
CSV_FILE_NAME = 'csvFileName'

# Default CSV export file name
DEFAULT_FILE_NAME = 'Shared_Tasks_for_{arch}_{date}_{time}.csv'

# Options for all shared tasks CSV plugins and scripts
CSV_OPTIONS = (
    Option(AUTO_EXPORT, 'Automatically export CSV file', OPTION_BOOL_CHOICES, OPTION_BOOL_FALSE),
    Option(CSV_FILE_LOCATION, 'CSV file location', [], ''),
    Option(CSV_FILE_NAME, 'CSV file name', [], DEFAULT_FILE_NAME),
) + COMMON_OPTIONS


def entComparator(a: Ent, b: Ent) -> int:
    if a.name() < b.name():
        return -1
    if a.name() > b.name():
        return 1
    return 0
entComparator = functools.cmp_to_key(entComparator)


def printEnt(report: IReport, ent: Ent, options: dict[str, str | bool]):
    report.entity(ent)
    report.print(getLongName(ent, options))
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
        objectName = getLongName(obj, options)
        shared = 'x' if len(fromTasksFiltered) > 1 else '-'

        # Make rows for each incoming edge to the object
        for edgeKey in edgeKeysForObjects[obj]:
            edgeObj = edgeInfo[edgeKey]

            # Info for each column for all rows of the edge
            protected = 'x' if refStr(edgeObj['ref']) in interruptDisabledRefs else '-'
            function = edgeObj['scope']
            functionName = getLongName(edgeObj['scope'], options)
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
                    printEnt(report, obj, options)
                    report.print(f', {shared}, {protected}, ')
                    printEnt(report, function, options)
                    report.print(f', {reference}, ')
                    printFile(report, file)
                    report.print(', ')
                    printEnt(report, task, options)
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
    file = open(os.path.join(options[CSV_FILE_LOCATION] or os.getcwd(), options[CSV_FILE_NAME]), 'w')
    file.writelines(lines)
    file.close()


##########################
# Command Line Interface #
##########################


# Required option keys
ARCH = 'arch'
DB = 'db'

# Options required for the CSV command line interface
REQUIRED_CLI_OPTIONS = (
    Option(ARCH, 'Architecture long name', [], ''),
    Option(DB, 'Path of .und folder', [], ''),
)


def printArguments(options: tuple[Option]):
    for option in options:
        # Key and value
        print(f'    -{option.key} VALUE')

        # Skip if no default value
        if not option.default:
            continue

        # Default value
        print(f'        Default: {option.default}')

        # Choices
        if option.choices == OPTION_BOOL_CHOICES:
            choices = "('False', 'True')"
        elif option.choices:
            choices = option.choices
        else:
            continue
        print(f'        Choices: {choices} (case-insensitive)')


def printHelpAndExit(expected: str | None = None, actual: str | None = None):
    if expected:
        print('Error parsing arguments:')
        print(f'    expected: {expected}')
        if actual:
            print(f'    actual:   "{actual}"')
        print('    help: [\'-h\']')

    else:
        print('Required arguments:')
        printArguments(REQUIRED_CLI_OPTIONS)
        print()
        print('Optional arguments:')
        printArguments(CSV_OPTIONS)

    exit()


def parseArguments() -> dict[str, str | bool]:
    result: dict[str, str | bool] = dict()

    # Get the CLI options as a dictionary
    optionDict: dict[str, Option] = dict()
    for option in REQUIRED_CLI_OPTIONS + CSV_OPTIONS:
        optionDict[f'-{option.key}'] = option

    # Get arguments
    argKey: str | None = None
    for arg in sys.argv[1:]:
        # Help argument
        if arg in {'-h', '-help', '--help', 'help'}:
            printHelpAndExit()

        # Argument key
        elif arg in optionDict:
            argKey = arg
            continue
        # Fail if there's no previous argument key
        elif not argKey:
            printHelpAndExit('key', arg)

        # Get the option information
        option: Option = optionDict[argKey]

        # Remove the leading dash
        optionKey = argKey[1:]

        # Remove quotes from the beginning and end
        if re.match(r'^\'.*\'$|^\".*\"$', arg):
            arg = arg[1:-1]

        # Option type: bool
        if option.choices == OPTION_BOOL_CHOICES:
            arg = arg.lower()
            if arg in {'true', '1'}:
                result[optionKey] = True
            elif arg in {'false', '0'}:
                result[optionKey] = False
            # Fail if the boolean value was bad
            else:
                printHelpAndExit(f'value for -{optionKey}', arg)
        # Option type: string from choices
        elif option.choices:
            argLower = arg.lower()
            allowedChoice = False
            for choice in option.choices:
                if argLower == choice.lower():
                    result[optionKey] = choice
                    allowedChoice = True
                    break
            # Fail if the choice was bad
            if not allowedChoice:
                printHelpAndExit(f'value for -{optionKey} from {option.choices}', arg)
        # Option type: string
        else:
            result[optionKey] = arg

        argKey = None

    # Fail if there are missing required arguments
    missingRequiredArgs: list[str] = []
    for option in REQUIRED_CLI_OPTIONS:
        if option.key not in result:
            missingRequiredArgs.append(f'-{option.key}')
    if missingRequiredArgs:
        printHelpAndExit(f'required arguments: {missingRequiredArgs}')

    # Add missing optional arguments
    for option in CSV_OPTIONS:
        if option.key not in result:
            result[option.key] = option.default

    return result


if __name__ == '__main__':
    # Parse arguments
    options = parseArguments()
    options[AUTO_EXPORT] = True

    # Get the database
    try:
        dbPath = options[DB]
        db = understand.open(dbPath)
    except:
        print('Error opening database:')
        print(f'    {dbPath}')
        exit(1)

    # Get the architecture
    archLongName = options[ARCH]
    arch = db.lookup_arch(archLongName)
    if not arch:
        print('Error getting architecture:')
        print(f'    {archLongName}')
        exit(1)

    # Export file
    generateCSV(db, arch, options)
