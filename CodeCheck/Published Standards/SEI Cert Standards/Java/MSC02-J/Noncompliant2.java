import java.util.Random;

public class NextInt1 {
    public static void main(String[] args)
    {
        // create random object
        Random ran = new Random();    // UNDCC_Violation, using Random to generate random number

        // generating integer
        int nxt = ran.nextInt();

        // Printing the random Number
        System.out.println
        ("The Randomly generated integer is : " + nxt);
    }
}
