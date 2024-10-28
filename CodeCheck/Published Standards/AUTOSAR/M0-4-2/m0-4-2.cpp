void foo()
{
    float a = 1.22;
    float b = 2.3;

    // UNDCC_Valid, has a comment before expression
    float compliant_1 = a * b;
    float compliant_2 = a * b; // UNDCC_Valid, has a comment
    float non_compliant = compliant_2 * (a * a);      // UndCC_Violation
    
}