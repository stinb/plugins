"""Module docstring for the PEP8_06 imports test."""
from __future__ import annotations

import os  # UndCC_Valid
import sys  # UndCC_Valid
import sys, os  # UndCC_Violation - multiple modules on one import
import collections, json, re  # UndCC_Violation - multiple modules
import os.path  # UndCC_Valid - dotted module, single import
import numpy as np, pandas as pd  # UndCC_Violation - aliases, still multiple

from subprocess import Popen, PIPE  # UndCC_Valid - from-import may list names
from mypkg import sibling  # UndCC_Valid
from mypkg.sibling import example  # UndCC_Valid
from . import sibling  # UndCC_Valid - relative import is acceptable
from .sibling import example  # UndCC_Valid

from os import *  # UndCC_Violation - wildcard import
from os.path import *  # UndCC_Violation - wildcard import

# Dunder assignments are allowed before imports
__all__ = ['a', 'b']
__version__ = '1.0'

import math  # UndCC_Valid - still only dunders/imports before this

# Imports may be wrapped in parentheses across lines
from collections import (
    OrderedDict,
    defaultdict,
)  # UndCC_Valid - parenthesized name list

# Conditional import guards are allowed before later top-level imports
try:
    import cjson as fastjson  # UndCC_Valid - nested, not module level
except ImportError:
    import json as fastjson  # UndCC_Valid - nested, not module level

import functools  # UndCC_Valid - try/except did not count as code


# Semicolon-separated statements are split and checked individually
import warnings; import textwrap  # UndCC_Valid - two single imports
import string; import glob, shutil  # UndCC_Violation - second statement imports multiple

# Backslash line continuations join into one logical statement
import \
    operator  # UndCC_Valid - single module across a continuation
from collections \
    import abc  # UndCC_Valid - from-import joined across a continuation


CONSTANT = 42  # this is real code at module level


import itertools  # UndCC_Violation - import after module-level code
from time import sleep  # UndCC_Violation - import after module-level code


def helper():
    import re  # UndCC_Valid - nested import not subject to top-of-file rule
    import os, sys  # UndCC_Violation - multiple, even when nested
    from os import *  # UndCC_Violation - wildcard, even when nested
    return re
