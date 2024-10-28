import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Operation_3 {
    public static void doOperation(String some_file) {
        // ... Code to check or set character encoding ...
        try {
            BufferedReader reader = new BufferedReader(new FileReader(some_file));
            try {
                // Do operations
            } finally { //UndCC_Violation
                reader.close();
                // ... Other cleanup code ...
            }
        } catch (IOException x) {
            // Forward to handler
        }
    }
}
