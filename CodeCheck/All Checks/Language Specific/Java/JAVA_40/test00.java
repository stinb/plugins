public enum State {
    /* UndCC_Valid(ignore_new_lines) */ RUNNING,
    /* UndCC_Valid(ignore_new_lines) */ PAUSED,
    /* UndCC_Valid(ignore_new_lines) */ STOPPED,
}

public class Program {
    public static void main(String[] args) {
        System.out.println("Hello", "world");      // UndCC_Valid
        System.out.println("Hello","world");       // UndCC_Valid(ignore_strings)
        // System.out.println("Hello","world");    // UndCC_Valid(ignore_comments)
        /* System.out.println("Hello","world"); */ // UndCC_Valid(ignore_comments)
        System.out.println(',');                   // UndCC_Valid(ignore_strings)
        System.out.println("Hello,world");         // UndCC_Valid(ignore_strings)
    }
}
