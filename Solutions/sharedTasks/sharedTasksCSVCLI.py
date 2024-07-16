# Command line interface that creates a CSV of shared tasks of an architecture


import os
import re
import sys

import understand

from sharedTasksCSVLib import *


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
        print(f'        {option.value}')

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
            print(f'    actual:   {actual}')
        print('    help: [\'-h\']')

    else:
        print('Required arguments:')
        printArguments(CSV_OPTIONS)
        print()
        print('Optional arguments:')
        printArguments(REQUIRED_CLI_OPTIONS)

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
        if arg in {'-h', '-help', 'help'}:
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
            lowerChoices = set(map(str.lower, option.choices))
            if arg in lowerChoices:
                result[optionKey] = arg
            # Fail if the choice was bad
            else:
                printHelpAndExit(f'value for -{optionKey}', arg)
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
