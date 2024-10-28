public class Noncompliant {
    String username = System.getenv("USER"); // UNDCC_Violation, getting a value from environment variable
}
