public class Noncompliant2 {
    boolean isCapitalized(String s) {
        if (s == null) {
            throw new RuntimeException("Null String"); // UNDCC_Violation, throws a RuntimeException
        }
        if (s.equals("")) {
            return true;
        }
        String first = s.substring(0, 1);
        String rest = s.substring(1);
        return (first.equals(first.toUpperCase()) &&
                rest.equals(rest.toLowerCase()));
    }

    private void doSomething() throws Throwable { // UNDCC_Violation, throws an Exception
        // ...
    }

}
