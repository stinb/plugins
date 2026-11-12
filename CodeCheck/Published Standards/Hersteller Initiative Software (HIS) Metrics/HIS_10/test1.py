import sys


def single_return(a):
    b = a + 1
    return b


def two_returns(a):  # UndCC_Violation
    if a:
        return 1
    return 2


def early_return(a):  # UndCC_Violation
    for i in a:
        return i
    print(a)


# Without a return statement the function returns None, like a void function
def no_return(a):
    print(a)


def bare_return(a):
    print(a)
    return


# raise is not a return point
def raises(a):
    if a:
        raise ValueError(a)
    return a


def exits(a):  # UndCC_Violation
    if a:
        sys.exit(1)
    return a


def generator(a):
    yield a
    yield a


async def fetch(a):  # UndCC_Violation
    if a:
        return 1
    return 2


def outer(a):
    def inner(b):  # UndCC_Violation
        if b:
            return 1
        return 2
    return inner(a)


class Thing:
    def __init__(self, a):  # UndCC_Violation(default)
        if not a:
            return
        self.a = a

    def __del__(self):  # UndCC_Violation(default)
        if self.a:
            return
        print(self)

    def get(self):
        return self.a


class Factory:
    def __new__(cls, a):  # UndCC_Violation(default)
        if a:
            return None
        return super().__new__(cls)


# Closing a with block or running a finally body still leaves the return last
def with_return(p):
    with open(p) as fh:
        return fh.read()


def nested_with(p, q):
    with open(p) as a:
        with open(q) as b:
            return a.read() + b.read()


def try_finally(a):
    try:
        return int(a)
    finally:
        print(a)


def try_finally_in_with(p):
    with open(p) as fh:
        try:
            return fh.read()
        finally:
            fh.close()


def return_in_finally(a):
    try:
        print(a)
    finally:
        return a


def with_then_more(p):  # UndCC_Violation
    with open(p) as fh:
        return fh.read()
    print(p)


def try_finally_then_more(a):  # UndCC_Violation
    try:
        return int(a)
    finally:
        print(a)
    print(a)


# The except handler follows the return, as with a C++ catch
def try_except(a):  # UndCC_Violation
    try:
        return int(a)
    except ValueError:
        raise
