def concat_bad(x: str, y: int) -> str:
    return x + y # UndCC_Violation


def concat_good(x: str, y: str) -> str:
    return x + y # UndCC_Valid
