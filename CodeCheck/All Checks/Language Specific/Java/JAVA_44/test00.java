import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class Program {
    public class RomanToInteger {
        private static Map<Character, Integer> map =
        new HashMap<Character, Integer>() {                // UNDCC_Valid
            {
                put('I', 1);
                put('V', 5);
                put('X', 10);
                put('L', 50);
                put('C', 100);
                put('D', 500);
                put('M', 1000);
            }
        };
    }

    public class Hello<T> {                                // UNDCC_Valid
        private static void f() {
            Vector<Integer> nums = new Vector<>(V);        // UNDCC_Valid
            int x = 2 + (-1 * 5);                          // UNDCC_Valid
            x =
                2 + (-1 * 5);                              // UNDCC_Valid
        }
    }

    public <T extends Comparable<T>> T[] sort(T[] array) { // UNDCC_Valid
        return new Comparator<T>() {                       // UNDCC_Valid
            public int compare(T a, T b) {
                return a - b;
            }
        };
    }

    public static void main(String[] args) {
        int x = 1;
        int y = 2;
        int z = 3;
        boolean b = true;
        String s = "Hello";

        z = x *y;                 // UNDCC_Violation
        z = x/ y;                 // UNDCC_Violation
        z = x%y;                  // UNDCC_Violation
        z = x+y;                  // UNDCC_Violation
        z = x-y;                  // UNDCC_Violation
        z = x<<y;                 // UNDCC_Violation
        z = x>>y;                 // UNDCC_Violation
        z = x>>>y;                // UNDCC_Violation
        b = x<y;                  // UNDCC_Violation
        b = x>y;                  // UNDCC_Violation
        b = x<=y;                 // UNDCC_Violation
        b = x>=y;                 // UNDCC_Violation
        b = (s)instanceof String; // UNDCC_Violation
        b = x==y;                 // UNDCC_Violation
        b = x!=y;                 // UNDCC_Violation
        z = x&y;                  // UNDCC_Violation
        z = x^y;                  // UNDCC_Violation
        z = x|y;                  // UNDCC_Violation
        b = b&&b;                 // UNDCC_Violation
        b = b||b;                 // UNDCC_Violation
        z=x;                      // UNDCC_Violation
        z+=x;                     // UNDCC_Violation
        z-=x;                     // UNDCC_Violation
        z*=x;                     // UNDCC_Violation
        z/=x;                     // UNDCC_Violation
        z%=x;                     // UNDCC_Violation
        z&=x;                     // UNDCC_Violation
        z^=x;                     // UNDCC_Violation
        z|=x;                     // UNDCC_Violation
        z<<=x;                    // UNDCC_Violation
        z>>=x;                    // UNDCC_Violation
        z>>>=x;                   // UNDCC_Violation
    }
}
