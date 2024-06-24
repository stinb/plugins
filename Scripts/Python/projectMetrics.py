# Sample Understand perl API program 
#
# Synopsis: Report project summary metrics
#
# Categories: Project Metrics
#
# Languages: All
#
# Written by Jason Quinn
#
# 1/28/21
#
import understand
import sys
import re
import argparse

# Usage:
parser = argparse.ArgumentParser()
parser.add_argument("db", help="Specify Understand database")
args = parser.parse_args()

if __name__ == '__main__':
    # Open Database
    db = understand.open(args.db)

    metrics = db.metric(db.metrics())


    for key, value in metrics.items():
        print(f'{key}:\t{value}'.expandtabs(37))
