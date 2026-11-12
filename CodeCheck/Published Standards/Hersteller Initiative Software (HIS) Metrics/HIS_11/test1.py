# Low VOCF: few repeated tokens
def add(a, b):
    return a + b


# High VOCF: the same few tokens repeated many times
def accumulate(values):  # UndCC_Violation - 7.27
    total = 0
    total = total + values[0]
    total = total + values[0]
    total = total + values[0]
    total = total + values[0]
    total = total + values[0]
    total = total + values[0]
    total = total + values[0]
    total = total + values[0]
    total = total + values[0]
    total = total + values[0]
    return total


class Shape:
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def scale(self, factor):  # UndCC_Violation - 6.23
        self.width = self.width * factor
        self.height = self.height * factor
        self.width = self.width * factor
        self.height = self.height * factor
        self.width = self.width * factor
        self.height = self.height * factor
        self.width = self.width * factor
        self.height = self.height * factor
        return self


# Strings and literals count as operands
def describe(kind):
    if kind == 1:
        return "one"
    if kind == 2:
        return "two"
    return "many"
