import datetime
import functools
import os
import re
import sys

import understand
from understand import Arch, Db, Ent, IReport

import sharedTasks


#######
# All #
#######


KEY, DESCRIPTION, DEFAULT, CHOICES = 0, 1, 2, 3
DEFAULT_FILE_NAME_TEMPLATE = 'Shared_Tasks_for_{arch}_{date}_{time}.csv'
requiredArguments = (
#    Key     Description
    ('arch', 'Architecture long name'),
    ('db',   'Path of .und folder'),
)
optionalArguments = (
#    Key                    Description                         Default                         Choices
    ('autoExport',          'Automatically export CSV file',    False),
    ('csvFileLocation',     'CSV file location',                os.getcwd()),
    ('csvFileName',         'CSV file name',                    DEFAULT_FILE_NAME_TEMPLATE),
    ('filterModifySetOnly', 'Filter out modify/set only',       False),
    ('filterUseOnly',       'Filter out use only',              False),
    ('objects',             'Object to show',                   'All',                          ['All', 'Shared only']),
    ('reference',           'References to show',               'All',                          ['All', 'Simple']),
)


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


def generateCSVRows(db: Db, arch: Arch, options: dict, lines: list[str], report: IReport | None):
    edgeInfo, tasks, incoming, interruptDisabledRefs, foundFields = sharedTasks.buildEdgeInfo(db, arch, options)

    # Sort the objects
    objects = set()
    for edgeObj in edgeInfo.values():
        ent = edgeObj['ent']
        if not ent.kind().check(sharedTasks.objEntKinds):
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
    headerFields = [field for field in sharedTasks.taskFields if field in foundFields]

    header = 'Global Object, Shared, Protected, Function, Reference(M=Modify/S=Set/U=Use), File, Task'
    if headerFields:
      header += ', ' + ', '.join(field.capitalize() for field in headerFields)
    header += '\n'

    if report:
        report.print(header)
    if lines is not None:
        lines.append(header)


    # Make rows for each object
    for obj in objects:

        # See how many tasks it is from
        fromTasks = set()
        fromTasksFiltered = set()
        edgeKeys = incoming[obj] if obj in incoming else set()
        for edgeKey in edgeKeys:
            info = edgeInfo[edgeKey]
            if not info['filtered'] and sharedTasks.refStr(info['ref']) not in interruptDisabledRefs:
                fromTasksFiltered.update(info['from'])
            fromTasks.update(info['from'])
        shared = len(fromTasks) > 1
        if options['objects'] == 'shared only' and not shared:
            continue

        # Info for each column for all rows of the object
        objectName = obj.name()
        shared = 'x' if len(fromTasksFiltered) > 1 else '-'

        # Make rows for each incoming edge to the object
        for edgeKey in edgeKeysForObjects[obj]:
            edgeObj = edgeInfo[edgeKey]

            # Info for each column for all rows of the edge
            protected = 'x' if sharedTasks.refStr(edgeObj['ref']) in interruptDisabledRefs else '-'
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


def generateCSV(db: Db, arch: Arch, options: dict, report: IReport | None = None):
    lines = []

    if not options['autoExport']:
        lines = None

    generateCSVRows(db, arch, options, lines, report)

    if options['autoExport']:
        # Default file name
        if not options['csvFileName']:
            options['csvFileName'] = DEFAULT_FILE_NAME_TEMPLATE

        # Substitute {arch}
        if re.search('{arch}', options['csvFileName'], flags=re.IGNORECASE):
            archSimplified = re.sub(r'\W|_', '', arch.name())
            options['csvFileName'] = re.sub('{arch}', archSimplified, options['csvFileName'], flags=re.IGNORECASE)

        # Substitute {date}
        now = None
        if re.search('{date}', options['csvFileName'], flags=re.IGNORECASE):
            if not now:
                now = datetime.datetime.now()
            date = now.strftime('%Y_%m_%d')
            options['csvFileName'] = re.sub('{date}', date, options['csvFileName'], flags=re.IGNORECASE)

        # Substitute {time}
        if re.search('{time}', options['csvFileName'], flags=re.IGNORECASE):
            if not now:
                now = datetime.datetime.now()
            time = now.strftime('%H_%M_%S')
            options['csvFileName'] = re.sub('{time}', time, options['csvFileName'], flags=re.IGNORECASE)

        # Append .csv to file name
        if not re.search(r'\.csv$', options['csvFileName']):
            options['csvFileName'] += '.csv'

        # Export lines to a new file
        file = open(os.path.join(options['csvFileLocation'], options['csvFileName']), 'w')
        file.writelines(lines)
        file.close()


#######
# CLI #
#######


def printArguments(arguments: tuple[tuple]):
    for option in arguments:
        # Key
        key = option[KEY]
        print(f'    -{key} VALUE')

        # Description
        description = option[DESCRIPTION]
        print(f'        {description}')

        # Default value
        if len(option) > DEFAULT:
            default = option[DEFAULT]
            print(f'        Default: {default}')

            # Choices
            if len(option) > CHOICES:
                choices = option[CHOICES]
            elif isinstance(default, bool):
                choices = '[\'False\', \'True\']'
            else:
                continue
            print(f'        Choices: {choices} (case-insensitive)')


def printHelpAndExit(expected: str | None = None, actual: str | None =None):
    if expected:
        print('Error parsing arguments:')
        print(f'    expected: {expected}')
        if actual:
            print(f'    actual:   {actual}')
        print('    help: [\'-h\']')

    else:
        print('Required arguments:')
        printArguments(requiredArguments)
        print()
        print('Optional arguments:')
        printArguments(optionalArguments)

    exit()


def parseArguments():
    # Argument keys, default values, allowed value choices
    keys = set()
    options = dict()
    choices = dict()
    for option in requiredArguments:
        keys.add(f'-{option[KEY]}')
    for option in optionalArguments:
        keys.add(f'-{option[KEY]}')
        options[option[KEY]] = option[DEFAULT]
        if len(option) > CHOICES:
            choices[option[KEY]] = set()
            for choice in option[CHOICES]:
                choices[option[KEY]].add(choice.lower())

    # Get arguments
    argKey = None
    for i in range(1, len(sys.argv)):
        arg = sys.argv[i]

        # Help argument
        if arg in {'-h', '-help', 'help'}:
            printHelpAndExit()

        # Argument key
        elif arg in keys:
            argKey = arg
            continue
        elif not argKey:
            printHelpAndExit('key', arg)

        # Argument value
        if argKey:
            argKey = argKey[1:]

            # Remove quotes from beginning/end
            if re.match(r'^\'.*\'$|^\".*\"$', arg):
                arg = arg[1:-1]

            # Type: bool
            if argKey in options and isinstance(options[argKey], bool):
                arg = arg.lower()
                if arg == 'true':
                    options[argKey] = True
                elif arg == 'false':
                    options[argKey] = False
                else:
                    printHelpAndExit(f'value for -{argKey}', arg)
            # Type: string from choices
            elif argKey in choices:
                arg = arg.lower()
                if arg in choices[argKey]:
                    options[argKey] = arg
                else:
                    printHelpAndExit(f'value for -{argKey}', arg)
            # Type: string
            else:
                options[argKey] = arg
        argKey = None

    # Required arguments
    missingArguments = []
    for option in requiredArguments:
        key = option[KEY]
        if key not in options:
            missingArguments.append(f'-{key}')
    if missingArguments:
        printHelpAndExit(f'required arguments: {missingArguments}')

    return options


if __name__ == '__main__':
    # Parse arguments
    optionalArguments = optionalArguments[1:]
    options = parseArguments()
    options['autoExport'] = True

    # Get the database
    try:
        dbPath = options['db']
        db = understand.open(dbPath)
    except:
        print('Error opening database:')
        print(f'    {dbPath}')
        exit(1)

    # Get the architecture
    archLongName = options['arch']
    arch = db.lookup_arch(archLongName)
    if not arch:
        print('Error getting architecture:')
        print(f'    {archLongName}')
        exit(1)

    # Export file
    generateCSV(db, arch, options)
