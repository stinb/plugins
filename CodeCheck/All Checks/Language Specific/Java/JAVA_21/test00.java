public class Test00 {
    private static void createAndFinalizeA() {
        Test00A test = new Test00A();
        System.out.println("\nFinalizing A");
        test.finalize();
        System.out.println("Finalized A");
    }
    private static void createAndFinalizeB() {
        Test00B test = new Test00B();
        System.out.println("\nFinalizing B");
        test.finalize();
        System.out.println("Finalized B");
    }

    public static void main(String[] args) {
        createAndFinalizeA();
        createAndFinalizeB();
    }
}

public class Test00A {
    public void finalize() throws ArithmeticException {
        try {
            divideByZero(); // UNDCC_Valid
        } catch (ArithmeticException e) {
            System.out.println(e);
        }
    }

    private void divideByZero() throws ArithmeticException {
        int x = 1 / 0;
    }
}

public class Test00B {
    public void finalize() throws ArithmeticException {
        divideByZero();     // UNDCC_Violation
    }

    private void divideByZero() throws ArithmeticException {
        int x = 1 / 0;
    }
}
