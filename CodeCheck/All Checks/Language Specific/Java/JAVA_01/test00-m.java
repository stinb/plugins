public class Test {
  
    public static final double PI = 3.14; // Undcc_Valid
  
    public static int fun(int x) {
        return x;
    }
    public static void main(String[] args) {
  
        final int z = fun(5 + (6 / fun(7))); // Undcc_Violation
        final int y = 2; // Undcc_Valid
        
        final int x; // Undcc_Valid
        x = 2; // Undcc_Valid
        
        int a = 0; // Undcc_Valid - via exceptions
        int b = 2; // Undcc_Violation
        
        int[] c = {10, 20, 30, 40}; // Undcc_Violation
        
        a = c[2]; // Undcc_Violation - may be toggled to be valid
        
        
        int ba = 0x2222; // Undcc_Violation
        final int bb = 0X2222; // Undcc_Valid
        
        int bv = bb << 10; // Undcc_Violation - may be toggled to be valid
        
    }
}