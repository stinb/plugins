import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class Compliant {
    public static void main(String[] args) {
        boolean validFlag = false;
        File ob;

        do {
            try {
                ob = new File("/aayush/home");
                FileInputStream fl = new FileInputStream(ob);
                // ...
                // If requested file does not exist, throws FileNotFoundException
                // If requested file exists, sets validFlag to true
                validFlag = true;
            } catch (FileNotFoundException e) { // UndCC_Valid, assuming code is not one-liner
                // Ask the user for a different file name
                ob = new File("new file name");
            }
        } while (validFlag != true);

    }
}
