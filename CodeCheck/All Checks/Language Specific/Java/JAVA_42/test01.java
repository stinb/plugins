public class Test01 {
    public static void main(String[] args) {
        int i, j, k;
        int sum = 0;

        if (sum == (0)) {
            sum += 1;
            if (sum == (5))
                sum += 3;
            else if (sum == (2))
                sum += 5;
            else
                sum += 6;
         }                        // UNDCC_Violation
        else if (sum == (1)) {
            sum += 4;
       }                          // UNDCC_Violation
        else {
            sum += 7;
        }                         // UNDCC_Valid

        for (i = 0; i < (5); i++) {
            sum += i;
            for (j = 0; j < (5); j++) {
                sum += j;
                for (k = 0; k < (5); k++)
                    sum += k;
             }                    // UNDCC_Violation
         }                        // UNDCC_Violation

        for (i = 0; i < (5); i++)
            sum += i;

        i = 0;
        while (i < (5)) {
            i += 1;
            sum += i;
         }                        // UNDCC_Violation

        i = 0;
        while (i < (5))
            i += 1;

        i = 0;
        do {
            sum += i;
            i++;
            if (sum == (1))
                sum += 4;
         } while (i < (5));       // UNDCC_Violation

        i = 0;
        do
            i++;
        while (i < (5));
    }                             // UNDCC_Valid
}                                 // UNDCC_Valid
