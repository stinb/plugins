import java.io.Console;

public class Noncompliant {
    public static void main(String[] args) {
        Console cnsl = null;
        String fmt = "%s";

        try {
            // ...
        } catch (SecurityException se) {
            cnsl = System.console();

            System.err.println(se); // UndCC_Violation
            cnsl.printf(fmt, se); // UndCC_Violation
            System.out.print(se); // UndCC_Violation
            System.out.println(se); // UndCC_Violation
            System.out.printf(fmt, se); // UndCC_Violation
            se.printStackTrace(); // UndCC_Violation

            // Recover from exception
        }
    }

}
