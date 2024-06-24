#
# Sample Understand Python API program 
#
# Synopsis: Outputs metrics for specified kinds of entities
#
# Categories: Project Metrics, Entity Metrics
#
# Languages: Ada, Fortran, C/C++, Java
#
# Written by Jason Quinn
#

# 1/22/21

import understand
import sys
import re
import argparse

# Usage:
parser = argparse.ArgumentParser()
parser.add_argument("db", help="Specify Understand database")
args = parser.parse_args()

# Gather and print the project metrics
def projectMetrics(db):
    metrics = db.metric(db.metrics())
    if metrics:
        print("Project Metrics:\n")

        for key, value in metrics.items():
            print(f'\t{key}: {value}')
        

# Gather and print metrics of each entity
def entityMetrics(db):
    ents = db.ents("~unresolved ~unknown")
    print("\nAll Entity Metrics:")

    # Sort entities by longname
    ents.sort(key=lambda x: x.longname())
    for ent in ents:
        metrics = ent.metric(ent.metrics())
        if metrics:
            print(f'\n    {ent.kindname()}: {ent.longname()} :    [File: {ent.ref().file().longname()} Line: {ent.ref().line()}]')
            metrics = ent.metric(ent.metrics())
            for key, value in metrics.items():
                print(f'\t{key}: {value}')


if __name__ == '__main__':
    # Open Database
    db = understand.open(args.db)

    projectMetrics(db)
    entityMetrics(db)

