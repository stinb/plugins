int main()
{
    int code[10];

    code[1] = 109;    // UndCC_Valid - decimal 109
    code[2] = 100;    // UndCC_Valid - decimal 100
    code[3] = 052;    // UndCC_Violation - equivalent to decimal 42, not 52
    code[4] = 071;    // UndCC_Violation - equivalent to decimal 57, not 71
    code[5] = 0;      // UndCC_Valid by exception
    code[6] = 000;    // UndCC_Violation - exception does not apply
    code[7] = '\123'; // UndCC_Violation(0)

    return 0;
}
