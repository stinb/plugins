import java.lang.Exception;

public class Test00 {
    static void caught() {
        try {
            int x = 1 / 0;
        }
        catch(Exception e) {             // UNDCC_Valid - Caught exception
            e.printStackTrace();
        }
    }

    static void declared1() throws Exception, ArithmeticException {
        throw new Exception();           // UNDCC_Violation
    }

    static void declared2() throws Exception, ArithmeticException {
        Exception e = new Exception();
        throw e;                         // UNDCC_Violation
    }

    static void declared3() throws Exception, ArithmeticException {
        throw new ArithmeticException(); // UNDCC_Valid - subclass
    }

    static void declared4() throws Exception, ArithmeticException {
        ArithmeticException e = new ArithmeticException();
        throw e;                         // UNDCC_Valid - subclass
    }

    static void declared5(Exception e) throws Exception, ArithmeticException {
        throw e;                         // UNDCC_Violation
    }

    static void declared6(ArithmeticException e) throws Exception, ArithmeticException {
        throw e;                         // UNDCC_Valid - subclass
    }

    public static void main(String[] args) throws Exception, ArithmeticException {

    }
}
