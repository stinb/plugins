public class Test01 {
    void f() {
        if (x == 0) y = 1; // UNDCC_Violation
        else        y = 2; // UNDCC_Violation

        do x += 1;    // UNDCC_Violation
        while(y == 0);

        switch (y) {  // UNDCC_Valid
        case 0: System.out.println("y is equal to 0"); break;   // UNDCC_Valid
        case 1: {System.out.println("y is equal to 1"); break;} // UNDCC_Valid
        default:
            System.out.println("y is either negative, or higher than 1");
            break;
        }

        if (y == 1)     // UNDCC_Violation



        x = 0;
        else {x = 1;}   // UNDCC_Valid

        if(x == 1) { y = 1; } else { y = 2;} // UNDCC_Valid
        if(x == 1) y = 1; else { y = 2;}     // UNDCC_Violation
        if(x == 1) y = x == 0 ? 1 : 2;       // UNDCC_Violation
    }
}
