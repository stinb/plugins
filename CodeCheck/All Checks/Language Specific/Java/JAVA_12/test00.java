public class Test00 {
    public static void main(String[] args) {
        // Test00B test = new Test00A();
        Test00B test = new Test00B();
        System.out.println("No errors thrown");
    }
}

public class Test00A {
    public Test00A() throws ArithmeticException {
        try {
            int x = 1 / 0;
        } catch (ArithmeticException e) { // UNDCC_Valid
            throw e;
        }
    }
}

public class Test00B {
    public Test00B() throws ArithmeticException {
        try {
            int x = 1 / 0;
        } catch (ArithmeticException e) { // UNDCC_Violation
            System.out.println(e);
        }
    }
}

