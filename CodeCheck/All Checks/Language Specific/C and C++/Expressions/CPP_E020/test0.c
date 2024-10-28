int main() {
    int code[ 10 ];

    code[ 1 ] = 109;      // UndCC_Valid     - equivalent to decimal 109
    code[ 2 ] = 100;      // UndCC_Valid     - equivalent to decimal 100
    code[ 3 ] = 052;      // UndCC_Violation - equivalent to decimal 42
    code[ 4 ] = 071;      // UndCC_Violation - equivalent to decimal 57
    code[ 5 ] = '\109';   // UndCC_Violation(0) - implementation-defined, two character constant
    code[ 6 ] = '\100';   // UndCC_Violation(0) - set to 64, or implementation-defined
    code[ 7 ] = 0;        // UndCC_Valid
    code[ 8 ] = '\0';     // UndCC_Valid
    code[ 9 ] = '\\213';  // UndCC_Valid - Escaped

    return 0;
}
