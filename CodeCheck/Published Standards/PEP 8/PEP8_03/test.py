# Test for PEP8_03. This file covers both option configurations:
# under style=Before (uvalid_options_0) trailing operators are flagged;
# under style=After (uvalid_options_1) leading operators are flagged.

gross_wages = 1
taxable_interest = 1
dividends = 1
qualified_dividends = 1
ira_deduction = 1
student_loan_interest = 1


# Wrong under Before: trailing operators
income = (gross_wages +  # UndCC_Violation(0)
          taxable_interest +  # UndCC_Violation(0)
          (dividends - qualified_dividends) -  # UndCC_Violation(0)
          ira_deduction -  # UndCC_Violation(0)
          student_loan_interest)


# Wrong under After: leading operators
income = (gross_wages
          + taxable_interest  # UndCC_Violation(1)
          + (dividends - qualified_dividends)  # UndCC_Violation(1)
          - ira_deduction  # UndCC_Violation(1)
          - student_loan_interest)  # UndCC_Violation(1)


# Comparison and boolean operators
if (gross_wages > 0 and  # UndCC_Violation(0)
        taxable_interest < 100):
    pass

if (gross_wages > 0
        and taxable_interest < 100):  # UndCC_Violation(1)
    pass


# Unary operators must NOT be flagged
x = (-gross_wages
     + taxable_interest)  # UndCC_Violation(1)

# Star/double-star as unpacking must NOT be flagged
def f(*args, **kwargs):
    return args, kwargs


# Backslash continuation (Before-style case is not representable with
# inline annotations because the violation fires on the line that ends
# in '\', where no trailing comment is allowed).
r2 = gross_wages \
    + taxable_interest  # UndCC_Violation(1)


# Nested brackets with mixed operators
y1 = (gross_wages + (dividends *  # UndCC_Violation(0)
                     qualified_dividends) +  # UndCC_Violation(0)
      taxable_interest)

y2 = (gross_wages
      + (dividends  # UndCC_Violation(1)
         * qualified_dividends)  # UndCC_Violation(1)
      + taxable_interest)  # UndCC_Violation(1)


# Compound operator: not in
items = [1, 2, 3]

if (5 not in  # UndCC_Violation(0)
        items):
    pass

if (5
        not in items):  # UndCC_Violation(1)
    pass


# Compound operator: is not
if (gross_wages is not  # UndCC_Violation(0)
        None):
    pass

if (gross_wages
        is not None):  # UndCC_Violation(1)
    pass


# Walrus operator
if (n :=  # UndCC_Violation(0)
        len(items)) > 10:
    pass

if (m
        := len(items)) > 10:  # UndCC_Violation(1)
    pass


# ** as dict unpacking inside a multi-line call must NOT be flagged
def _g(**kwargs):
    return kwargs

kwargs1 = {'a': 1}
_g(
    x=1,
    **kwargs1,
)


# Decorators must NOT be flagged, even when preceded by an operand on
# the previous line (which could otherwise trick @ into looking binary).
def _deco(fn):
    return fn

@_deco
def _dec_once():
    pass

@_deco
@_deco
def _dec_twice():
    pass


# Matrix multiplication operator (@) used as binary
matrix_a = 1
matrix_b = 1
matrix_c = (matrix_a @  # UndCC_Violation(0)
            matrix_b)

matrix_d = (matrix_a
            @ matrix_b)  # UndCC_Violation(1)


# Lambda split across lines
square1 = lambda v: (v *  # UndCC_Violation(0)
                     v)

square2 = lambda v: (v
                     * v)  # UndCC_Violation(1)
