"""Module docstring for PEP8_04 blank-lines test."""
import os
import sys


def first_top_level():  # UndCC_Valid - 2 blank lines after imports
    pass


def second_top_level():  # UndCC_Valid - 2 blank lines before
    pass

def too_close_top_level():  # UndCC_Violation - only 1 blank line before
    pass



def extra_blanks_ok():  # UndCC_Valid - 3 blanks is fine (minimum rule)
    pass


class MyClass:  # UndCC_Valid - 2 blank lines before class

    def first_method(self):  # UndCC_Valid - first item in class body
        pass

    def second_method(self):  # UndCC_Valid - 1 blank line before
        pass
    def third_method(self):  # UndCC_Violation - 0 blank lines before
        pass


    def two_blanks_before(self):  # UndCC_Valid - extra blanks allowed
        pass

    @staticmethod
    def decorated(x):  # UndCC_Valid - 1 blank before decorator
        return x

    @classmethod
    @staticmethod
    def multi_decorated(cls):  # UndCC_Valid - 1 blank before first decorator
        pass
    @classmethod
    def decorated_no_blank(cls):  # UndCC_Violation - 0 blanks before decorator
        pass

    # An attached comment block
    # spanning two lines
    def commented_method(self):  # UndCC_Valid - 1 blank before comment block
        pass

    class Inner:  # UndCC_Valid - nested class, 1 blank before, OK

        def inner_method(self):  # UndCC_Valid - first item in Inner's body
            pass
        def inner_method_two(self):  # UndCC_Violation - 0 blanks before method
            pass

class AnotherClass:  # UndCC_Violation - only 1 blank line before top-level class
    pass

@decorator_func
def decorated_top_level():  # UndCC_Violation - only 1 blank before decorator
    pass


@decorator_func(
    arg1=1,
    arg2=2,
)
def multi_line_decorator():  # UndCC_Valid - 2 blanks before decorator start
    pass


def outer():  # UndCC_Valid
    def inner():  # UndCC_Valid - nested inside function, not checked
        pass
    def another_inner():  # UndCC_Valid - nested inside function, not checked
        pass

    class InnerClass:  # UndCC_Valid - nested inside function, not checked
        def x(self):  # UndCC_Valid - nested inside function, not checked
            pass
        def y(self):  # UndCC_Valid - nested inside function, not checked
            pass


# Attached comment for a top-level definition
def after_comment_block():  # UndCC_Valid - 2 blanks before comment block
    pass


x_between = 1
# Another attached comment
# on two lines
def after_comment_block_two():  # UndCC_Violation - 0 blanks before comments
    pass


class ClassWithPrecedingVar:  # UndCC_Valid - 2 blanks before class

    some_class_var = 1
    def method_after_var(self):  # UndCC_Violation - 0 blanks after class var
        pass

    another_var = 2

    def method_after_var_with_blank(self):  # UndCC_Valid - 1 blank after var
        pass


def decorator_func(fn):  # UndCC_Valid - 2 blanks before (after class body)
    return fn
