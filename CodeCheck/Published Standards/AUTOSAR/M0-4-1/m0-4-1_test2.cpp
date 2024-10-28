
const int scale = 8;

#define DoubleToFixed2(x) (x * (double)(1 << scale))

int main()
{
    int fixed_1 = DoubleToFixed2(24.8);
    int fixed_2 = DoubleToFixed2(7.32);

    int compliant = fixed_1 * fixed_2; // UNDCC_Valid - has inline comment
    // UndCC_Valid - has above comment
    int compliant2 = fixed_1 * fixed_2; 
    
    
    
    
    int non_compliant = fixed_1 + fixed_2; // UndCC_Violation
    
}
