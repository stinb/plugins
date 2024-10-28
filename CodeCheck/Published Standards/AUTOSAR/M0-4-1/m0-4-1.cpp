
const int scale = 16;

#define DoubleToFixed(x) (x * (double)(1 << scale))

int main()
{
    int fixed_1 = DoubleToFixed(24.8);
    int fixed_2 = DoubleToFixed(7.32);

    // UNDCC_Valid, has a comment before expression
    int compliant_1 = fixed_1 * fixed_2;
    int compliant_2 = fixed_1 * fixed_2; // UNDCC_Valid, has a comment
    
    
    
    
    

    int non_compliant = fixed_1 + fixed_2; // UndCC_Violation
    
}
