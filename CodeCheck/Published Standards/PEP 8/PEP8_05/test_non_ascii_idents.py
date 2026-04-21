"""Non-ASCII identifiers are only flagged when flagNonAsciiIdentifiers is on."""


café = 1  # UndCC_Violation(1)


def π(résultat):  # UndCC_Violation(1)
    return résultat


class Żółw:  # UndCC_Violation(1)
    pass


plain_ascii = 0  # UndCC_Valid
