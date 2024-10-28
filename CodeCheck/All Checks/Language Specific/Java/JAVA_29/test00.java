public class Test00 {
    public static void main(String[] args) {
        int i, j, k;
        int sum = 0;

        if (sum == (0)) {                 // UNDCC_Valid
            sum += 1;
            if (sum == (5))               // UNDCC_Violation
                sum += 3;
            else if (sum == (2))          // UNDCC_Violation
                sum += 5;
            else                          // UNDCC_Violation
                sum += 6;
        } else if (sum == (1)) {          // UNDCC_Valid
            sum += 4;
        } else {                          // UNDCC_Valid
            sum += 7;
        }

        for (i = 0; i < (5); i++) {       // UNDCC_Valid
            sum += i;
            for (j = 0; j < (5); j++) {   // UNDCC_Valid
                sum += j;
                for (k = 0; k < (5); k++) // UNDCC_Violation
                    sum += k;
            }
        }

        for (i = 0; i < (5); i++)         // UNDCC_Violation
            sum += i;

        i = 0;
        while (i < (5)) {                 // UNDCC_Valid
            i += 1;
            sum += i;
        }

        i = 0;
        while (i < (5))                   // UNDCC_Violation
            i += 1;

        i = 0;
        do {                              // UNDCC_Valid
            sum += i;
            i++;
            if (sum == (1))               // UNDCC_Violation
                sum += 4;
        } while (i < (5));                // UNDCC_Valid

        i = 0;
        do                                // UNDCC_Violation
            i++;
        while (i < (5));
    }
}
