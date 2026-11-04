import understand
import sys
import os

from pathlib import PurePath
from subprocess import run

if __name__ == '__main__':
    # Open Database
    args = sys.argv
    db = understand.open(args[1])

    file_arch = {}
    input_file = args[2]

    if os.path.exists(input_file):
        with open(input_file, 'r') as f:
            lines = f.readlines()

        file_arch = dict(line.strip().split(',') for line in lines)

    for file in db.ents("file ~unknown ~unresolved"):
        fullpath = PurePath(file.longname())
        fullpath = str(fullpath)
        arch_name = file_arch.get(fullpath)

        if arch_name != None and os.path.exists(fullpath):
            file.add_to_arch(arch_name)
