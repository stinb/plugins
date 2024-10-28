public enum Status {
    STATE_0,              // UndCC_Valid
    STATE_1,              // UndCC_Valid
    STATE_2,              // UndCC_Valid
}

public class Program {
    public static void main(String[] args) {
        int a, b, c;      // UndCC_Violation
        int d, e[];       // UndCC_Violation
        int f[], g[];     // UndCC_Violation
        int h[], i;       // UndCC_Violation
        int j = 1, k = 2; // UndCC_Violation
    }
}
