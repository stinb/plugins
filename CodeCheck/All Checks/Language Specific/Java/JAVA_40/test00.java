public enum State {
    RUNNING, // UndCC_Valid - only when ignoring new lines
    PAUSED,  // UndCC_Valid - only when ignoring new lines
    STOPPED, // UndCC_Valid - only when ignoring new lines
}

public class Program {
    public static void main(String[] args) {
        System.out.println("Hello", "world");      // UndCC_Valid
        System.out.println("Hello","world");       // UndCC_Violation
        // System.out.println("Hello","world");    // UndCC_Valid - only when ignoring comments
        /* System.out.println("Hello","world"); */ // UndCC_Valid - only when ignoring comments
        System.out.println(',');                   // UndCC_Valid - only when ignoring strings
        System.out.println("Hello,world");         // UndCC_Valid - only when ignoring strings
    }
}
