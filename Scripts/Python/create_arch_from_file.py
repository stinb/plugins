from pathlib import PurePath
from subprocess import run
import understand
import sys
import os
input_file = ''


if __name__ == '__main__':
    # Open Database
    args = sys.argv
    db = understand.open(args[1])

    fileValues = {}
    input_file = "input_arch.csv"

    if os.path.exists(input_file):
        with open(input_file, 'r') as f:
            lines = f.readlines()
        fileValues = dict(line.strip().split(',') for line in lines)
        for file in db.ents("file ~unknown ~unresolved"):

            fullpath = PurePath(file.longname())
            fullpath = str(fullpath)
            for key, value in fileValues.items():
                # print(key, value)
                if key == fullpath:
                    archval = fileValues.get(fullpath)

                    if archval != None and os.path.exists(fullpath):
                        print("Add File to Architecture", file.name(), archval)
                        file.add_to_arch(archval)
                        print("___")
