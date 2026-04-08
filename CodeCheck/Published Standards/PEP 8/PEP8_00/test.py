# Correct indentation
def hello():  # UndCC_Valid
    print("hello")  # UndCC_Valid


def add(a, b):  # UndCC_Valid
    if a > 0:  # UndCC_Valid
        if b > 0:  # UndCC_Valid
            return a + b  # UndCC_Valid
    return 0  # UndCC_Valid


class MyClass:  # UndCC_Valid
    def __init__(self):  # UndCC_Valid
        self.value = 0  # UndCC_Valid

    def method(self):  # UndCC_Valid
        pass  # UndCC_Valid


# Continuation lines - any indentation is fine
result = dict(
  one=1,
  two=2,
)

foo = long_function_name(
    var_one, var_two,
    var_three, var_four)

my_list = [
    1, 2, 3,
    4, 5, 6,
]

if (True
        and True):
    pass  # UndCC_Valid


# Incorrect indentation
def bad_hello():  # UndCC_Valid
  print("hello")  # UndCC_Violation

def bad_add(a, b):  # UndCC_Valid
   if a > 0:  # UndCC_Violation
      return a + b  # UndCC_Violation
   return 0  # UndCC_Violation

class BadClass:  # UndCC_Valid
  def __init__(self):  # UndCC_Violation
    self.value = 0  # UndCC_Valid

  def method(self):  # UndCC_Violation
   pass  # UndCC_Violation

def tabbed():  # UndCC_Valid
	print("tabs")  # UndCC_Violation
