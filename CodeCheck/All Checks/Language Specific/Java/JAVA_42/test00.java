public class Test00 {
    static f() {

        for (int i=0; i<10; i++) {

        }                                        // UNDCC_Valid

        for (int i=0; i<10; i++) {

            }                                    // UNDCC_Violation

        for (int i=0; i<10; i++) {}              // UNDCC_Violation

        int i = 0;
        do {
            i++;
        }                                        // UNDCC_Valid
        while (i<10);

        i = 0;
        do {
            i++;
         }                                       // UNDCC_Violation
        while (i<10);

        char[][] chars = new char[][] {
            {'t', 'h', 'i', 's', 'i', 's', 'a'}, // UNDCC_Valid
            {'s', 'i', 'm', 'p', 'l', 'e', 'x'}, // UNDCC_Valid
            {'b', 'x', 'x', 'x', 'x', 'e', 'b'}, // UNDCC_Valid
            {'x', 'o', 'g', 'g', 'l', 'x', 'o'}, // UNDCC_Valid
            {'x', 'x', 'x', 'D', 'T', 'r', 'a'}, // UNDCC_Valid
            {'R', 'E', 'P', 'E', 'A', 'd', 'x'}, // UNDCC_Valid
            {'x', 'x', 'x', 'x', 'x', 'x', 'x'}, // UNDCC_Valid
            {'N', 'O', 'T', 'R', 'E', '_', 'P'}, // UNDCC_Valid
            {'x', 'x', 'D', 'E', 'T', 'A', 'E'}, // UNDCC_Valid
        };                                       // UNDCC_Valid



        char[][][] chars = new char[][][] {
            {
                {
                    't', 'h', 'i', 's', 'i', 's', 'a'
                },                               // UNDCC_Valid
            },                                   // UNDCC_Valid
        };                                       // UNDCC_Valid


    }                                            // UNDCC_Valid

    static g() {

   }                                             // UNDCC_Violation

    static h() {

     }                                           // UNDCC_Violation

    static class A {

    }                                            // UNDCC_Valid

    static class B {

}                                                // UNDCC_Violation
    static class C {

        }                                        // UNDCC_Violation
}                                                // UNDCC_Valid
