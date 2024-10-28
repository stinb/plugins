public class Compliant {
    public static void main(String[] args) {
        final int authnum = get();
        int number = 17;
        int threshold = 10;
        number = ((31 * (number + 1)) * authnum) + (authnum > threshold ? 0 : -2);
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
