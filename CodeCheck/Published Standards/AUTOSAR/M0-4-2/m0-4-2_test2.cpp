void bar()
{
    float w = 1.22;
    float x = 2.3;
    float y = w * x;       // UndCC_Violation
    
     
    // UNDCC_Valid, above comment compliant for y and z
    float z = y * (w * w);

    float aa = w * z; // UNDCC_Valid, compliant with comment inline
    
    float non_compliant = aa * (z * z);
    
}
