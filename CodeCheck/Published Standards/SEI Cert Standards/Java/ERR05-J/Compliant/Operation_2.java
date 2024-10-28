import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Operation_2 {
    public static void doOperation(String some_file) {
        // ... Code to check or set character encoding ...
        try ( //UndCC_Valid - try-with-resources
                BufferedReader reader = new BufferedReader(new FileReader(some_file))) {
            // Do operations
        } catch (IOException ex) {
            System.err.println("thrown exception: " + ex.toString());
            Throwable[] suppressed = ex.getSuppressed();
            for (int i = 0; i < suppressed.length; i++) {
                System.err.println("suppressed exception: "
                        + suppressed[i].toString());
            }
            // Forward to handler
        }
    }

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Please supply a path as an argument");
            return;
        }
        doOperation(args[0]);
    }
}
