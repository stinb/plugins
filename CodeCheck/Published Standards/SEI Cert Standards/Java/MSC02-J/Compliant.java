import java.security.SecureRandom;

public class Compliant {
    public static void main(String args[]) {
        SecureRandom number = new SecureRandom();
        // Generate 20 integers 0..20
        for (int i = 0; i < 20; i++) {
            System.out.println(number.nextInt(21));
        }
    }
}
