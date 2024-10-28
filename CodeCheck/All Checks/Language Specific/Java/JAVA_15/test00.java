public class Main {
    public void FunctionOne() {    // UNDCC_Violation

    }

    public void function_two() {   // UNDCC_Violation

    }

    public void Function_Three() { // UNDCC_Violation

    }

    public void functionFour() {   // UNDCC_Valid
        int x = () -> {            // UNDCC_Valid
            return 0;
        };
    }
}
