import java.util.Random;

public class Noncompliant {
    public static void main(String args[]) {
        Random number = new Random(123L); // UNDCC_Violation, using Random to generate random number

        for (int i = 0; i < 20; i++) {
            // Generate another random integer in the range [0, 20]
            int n = number.nextInt(21);
            System.out.println(n);
        }

    }

}
