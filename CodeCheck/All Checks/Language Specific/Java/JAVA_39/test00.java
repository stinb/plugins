public class Program {
    public static void main(String[] args) {
        int[] a = new int[10];
        int x = 1;
        int y = 2;
        boolean b = true;
        boolean c = false;
        String s = "s";
        String t = "t";

        a[0] = 0;  // UNDCC_Valid
        a [1] = 1; // UNDCC_Violation

        x++;  // UNDCC_Valid
        ++x;  // UNDCC_Valid
        x ++; // UNDCC_Violation
        ++ x; // UNDCC_Violation

        x--;  // UNDCC_Valid
        --x;  // UNDCC_Valid
        x --; // UNDCC_Violation
        -- x; // UNDCC_Violation

        t = "" + s; // UNDCC_Valid
        y = 1 + x;  // UNDCC_Valid
        y = x + x;  // UNDCC_Valid
        y = x - x;  // UNDCC_Valid
        y = 1 - x;  // UNDCC_Valid

        y = +x;  // UNDCC_Valid
        y = + x; // UNDCC_Violation

        y = -x;  // UNDCC_Valid
        y = - x; // UNDCC_Violation

        c = ~b;  // UNDCC_Valid
        c = ~ b; // UNDCC_Violation

        c = !b;  // UNDCC_Valid
        c = ! b; // UNDCC_Violation
    }
}
