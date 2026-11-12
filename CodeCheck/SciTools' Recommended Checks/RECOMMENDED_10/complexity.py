# Modified cyclomatic complexity 22, over the default maximum of 20
def too_complex(a):  # UndCC_Violation
    if a == 1: return 1
    if a == 2: return 2
    if a == 3: return 3
    if a == 4: return 4
    if a == 5: return 5
    if a == 6: return 6
    if a == 7: return 7
    if a == 8: return 8
    if a == 9: return 9
    if a == 10: return 10
    if a == 11: return 11
    if a == 12: return 12
    if a == 13: return 13
    if a == 14: return 14
    if a == 15: return 15
    if a == 16: return 16
    if a == 17: return 17
    if a == 18: return 18
    if a == 19: return 19
    if a == 20: return 20
    if a == 21: return 21
    return 0


# Modified cyclomatic complexity 20, at the maximum
def at_limit(a):
    if a == 1: return 1
    if a == 2: return 2
    if a == 3: return 3
    if a == 4: return 4
    if a == 5: return 5
    if a == 6: return 6
    if a == 7: return 7
    if a == 8: return 8
    if a == 9: return 9
    if a == 10: return 10
    if a == 11: return 11
    if a == 12: return 12
    if a == 13: return 13
    if a == 14: return 14
    if a == 15: return 15
    if a == 16: return 16
    if a == 17: return 17
    if a == 18: return 18
    if a == 19: return 19
    return 0


class Holder:
    def too_complex_method(self, a):  # UndCC_Violation
        if a == 1: return 1
        if a == 2: return 2
        if a == 3: return 3
        if a == 4: return 4
        if a == 5: return 5
        if a == 6: return 6
        if a == 7: return 7
        if a == 8: return 8
        if a == 9: return 9
        if a == 10: return 10
        if a == 11: return 11
        if a == 12: return 12
        if a == 13: return 13
        if a == 14: return 14
        if a == 15: return 15
        if a == 16: return 16
        if a == 17: return 17
        if a == 18: return 18
        if a == 19: return 19
        if a == 20: return 20
        if a == 21: return 21
        return 0
