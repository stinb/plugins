public class Test00 {
    public static void main(String[] args) {
        int n = 0;
        char x;

        x = n % 2 == 0 ? 'a' // UNDCC_Valid
          : n % 3 == 0 ? 'b' // UNDCC_Violation
          : n % 5 == 0 ? 'c' // UNDCC_Violation
          : n % 7 == 0 ? 'd' // UNDCC_Violation
          : 'e';

        x = n % 2 == 0 ? 'a' : 'b'; // UNDCC_Valid
        x = n % 3 == 0 ? 'c' : 'd'; // UNDCC_Valid

        int[] integers = new int[] {
            n % 2 == 0 ? 1 : 2,   // UNDCC_Valid
            n % 3 == 0 ? 3 : 4,   // UNDCC_Valid
            n % 5 == 0 ? 5 : 6    // UNDCC_Valid
        };
    }
}
