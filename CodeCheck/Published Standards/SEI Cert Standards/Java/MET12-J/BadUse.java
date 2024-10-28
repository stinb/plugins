import java.lang.System;

public class BadUse {
    public static void main(String[] args) {
        try {
            BaseClass bc = new SubClass();
            // Artificially simulate finalization (do not do this)
            System.runFinalizersOnExit(true); // UndCC_Violation
        } catch (Throwable t) {
            // Handle error
        }
    }
}