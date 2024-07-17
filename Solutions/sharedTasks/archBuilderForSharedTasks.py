#! /usr/bin/env upython


# Command line interface to create architectures for the Shared Tasks graph


import sys

import understand
from understand import Ent


def error(string: str):
    print(f'\033[91m{string}\033[0m')


def getNum(prompt: str):
    while True:
        num = input(string)
        if num:
            try:
                num = int(num)
            except:
                error('Not a valid number')
                continue
        else:
            num = None
        break
    return num


def getFunction(functions: dict[str, list[Ent]], functionType: str):
    while True:
        name = input(f'{functionType} function name: ')
        # Try again if function found
        if name not in functions:
            error('Not found')
            continue
        # If not ambiguous, number is 0
        if len(functions[name]) == 1:
            n = 0
        # If ambiguous, list duplicate functions and get number
        else:
            print('Duplicates:')
            for i in range(len(functions[name])):
                print(f'({i}) {functions[name][i].uniquename()}')
            n = num('Function number: ', 0, len(functions[name]) - 1)
        return functions[name][n]


def getChar(prompt: str, chars: set[str]):
    while True:
        char = input(prompt)
        if char:
            char = char[0].lower()
            if char in chars:
                return char


def main():
    print('Quit anytime with "control c"')

    # Get user input for database
    db = None
    if len(sys.argv) >= 2:
        db = sys.argv[1]
    while True:
        db = db or input('\nDatabase path: ')
        try:
            db = understand.open(db)
        except:
            error('Error opening database')
            error(e)
            db = None
            continue
        break

    # Make a dictionary { name: [ ent, ... ], ... }
    functions = dict()
    for ent in db.ents('Function'):
        name = ent.name()
        if name not in functions:
            functions[name] = []
        functions[name].append(ent)

    # Get user input for architecture name
    if len(sys.argv) >= 3:
        arch = sys.argv[2]
    else:
        while True:
            print()

            # Architecture
            arch = input('Architecture longname: ')
            if not arch:
                error('No name')
                continue
            if arch == 'Directory Structure':
                error('Invalid name')
                continue
            break

    # Get user input for each function
    while True:
        print()

        # Task or interrupt control
        char = getChar('(T)ask or (I)nterrupt Control: ', {'t', 'i'})

        # Task
        if char == 't':
            # Group: task priority
            group = getNum('Task priority: ')

            # Function: task
            task = getFunction(functions, 'Task')
            if group:
                task.add_to_arch(f'{arch}/Task Priority/{group}')
            else:
                task.add_to_arch(f'{arch}/Tasks')

        # Interrupt control
        else:
            # Group: Interrupt control
            group = input('Interrupt control name: ')

            # Function: disable
            disable = getFunction(functions, 'Disable')
            disable.add_to_arch(f'{arch}/Interrupt Control/{group}/Disable')

            # Function: enable
            enable = getFunction(functions, 'Enable')
            enable.add_to_arch(f'{arch}/Interrupt Control/{group}/Enable')


if __name__ == '__main__':
    main()
