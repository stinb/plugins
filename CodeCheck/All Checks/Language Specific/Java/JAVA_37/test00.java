public class Test00 {
    public static void main(String[] args) {
        int i, x, y;

        final int finalOne = 1;
        final int finalTwo = 2;
        int varThree = 3;
        int varFour = 3;
        varFour = 4;

        y = 0;                                // UNDCC_Valid

        for (i = 0; i < 3; i++) {             // UNDCC_Valid
            x = i * 2 + i;                    // UNDCC_Valid
            x = finalOne - finalTwo;          // UNDCC_Violation
            x = finalOne - 2;                 // UNDCC_Violation
            x = varThree - finalOne;          // UNDCC_FalseNeg - Variable value not determined with static analysis
            x = varFour - 1;                  // UNDCC_FalseNeg - Variable value not determined with static analysis
        }

        y = 0;                                // UNDCC_Valid

        for (i = 0; i < 3; i++)               // UNDCC_Valid
            x = 1 + 2;                        // UNDCC_Violation

        i = 0;                                // UNDCC_Valid
        while (i < 3) {
            x = 2 / 3;                        // UNDCC_Violation
            i++;
        }

        y = 0;                                // UNDCC_Valid

        while (false)
            x = 4 % 2 == 0 ? 1 : 0;           // UNDCC_Violation


        i = 0;                                // UNDCC_Valid
        do {
            x = 3;                            // UNDCC_Violation
            i++;
        } while (i < 3);

        y = 0;                                // UNDCC_Valid

        do
            x = 0;                            // UNDCC_Violation
        while (false);


        i = 0;                                // UNDCC_Valid
        do {
            x = 0;                            // UNDCC_Violation
            i++;
            {
                y = 3;                        // UNDCC_Violation
            }
            for (i = 0; i < 3; i++) {         // UNDCC_Valid
                x = 0;                        // UNDCC_Violation

                do {
                    x = 0;                    // UNDCC_Violation
                    i++;
                    {
                        y = 3;                // UNDCC_Violation
                    }
                    for (i = 0; i < 3; i++) { // UNDCC_Valid
                        x = 0;                // UNDCC_Violation
                    }
                }
                while (i < 3);
            }
        }
        while (i < 3);

        for (;;) ;
        x = 3;                                // UNDCC_Valid
    }

    public static void f() {
        int x = 3;                            // UNDCC_Valid
    }
}
