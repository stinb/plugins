public class Noncompliant2 {
    String username = System.getenv("HOME"); // UNDCC_Violation, getting a value from environment variable
    String username = System.getenv("LOGNAME"); // UNDCC_Violation, getting a value from environment variable
}
