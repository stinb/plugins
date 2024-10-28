class Noncompliant {
    public static void main(String[] args) {
        int number = 17;
        int threshold = 10;
        number = (number > threshold ? 0 : -2) + ((31 * ++number) * (number = get())); // UNDCC_Violation
        // ...
        if (number == 0) {
            System.out.println("Access granted");
        } else {
            System.out.println("Denied access"); // number = -2
        }
    }

    public static int get() {
        int number = 0;
        // Assign number to nonzero value if authorized, else 0
        return number;
    }
}
