# Short lines
x = 1  # UndCC_Valid
name = "hello"  # UndCC_Valid

# Code line at exactly 79 characters (ok)
long_variable = "this line is padded to be exactly seventy nine"  # UndCC_Valid

# Code line at exactly 80 characters (violation)
long_variable = "this is padded to be line eighty characters"  # UndCC_Violation

# Comment at exactly 72 characters (ok)
# This comment is padded to be seventy two characters ok!  # UndCC_Valid

# Comment at exactly 73 characters (violation)
# This comment is padded to be seventy three chars ok!  # UndCC_Violation

# Single-line docstring at exactly 72 characters (ok)
"""This docstring is exactly at the seventy two limit!"""  # UndCC_Valid

# Single-line docstring at exactly 73 characters (violation)
"""This single line docstring exceeds the limit!!!!"""  # UndCC_Violation

# URL-only comment lines should be excluded regardless of length
# https://docs.scitools.com/manuals/python/index.html?highlight=lexer#understand.File.lexer

# Multi-line docstring with a long interior line
def example():
    """
    Short line.
    This line is inside a multi-line docstring.
    """
    pass  # UndCC_Valid
