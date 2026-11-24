"""Module docstring for the PEP8_07 dunder names test."""
from __future__ import (
    annotations,
    division,
)

__all__ = ['a', 'b']        # UndCC_Valid - dunder before any import
__version__ = '0.1'         # UndCC_Valid - dunder before any import
__author__ = 'Cardinal Biggles'  # UndCC_Valid


def helper():
    import os
    __local__ = 1           # UndCC_Valid - not module level (inside function)
    return os, __local__


async def async_helper():
    import os
    return os


class C:
    import sys
    __slots__ = ('x',)      # UndCC_Valid - not module level (inside class)

    def __init__(self):
        self.x = 0


__credits__ = ['x']         # UndCC_Valid - only function/class imports precede
__name__ == '__main__'      # UndCC_Valid - comparison, not assignment

try:
    import json
except ImportError:
    json = None

__revision__ = '42'         # UndCC_Violation - after an import in a try block

import os
import sys

from collections import OrderedDict
from doctest import *

__test__ = {}               # UndCC_Violation - after a from-import
__docformat__ = 'rst'       # UndCC_Violation - after a star import

CONSTANT = 99

__maintainer__ = 'Jane'     # UndCC_Violation - after imports and other code

__all__ += ['c']            # UndCC_Valid - augmented assignment
__all__.extend(['d'])       # UndCC_Valid - method call
__status__: str = 'Beta'    # UndCC_Violation - annotated assignment
__a__, rest = 1, 2          # UndCC_Violation - tuple target
rest, __b__ = 1, 2          # UndCC_Violation - tuple target, second name
__c__, *more = 1, 2         # UndCC_Violation - starred tuple target
x = 1; __d__ = 2            # UndCC_Violation - after a semicolon
__all__[0] = 'x'            # UndCC_Violation - item assignment
C.__repr__ = lambda s: 'c'  # UndCC_Valid - attribute of a class
os.__dunder__ = 1           # UndCC_Valid - attribute of another module
vals = [__x__ for __x__ in range(3)]  # UndCC_Valid - comprehension variable
__ann__: list               # UndCC_Valid - bare annotation, no value
