import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class Noncompliant {
    public static void main(String[] args) {
        try {
            File ob = new File("/aayush/home");
            FileInputStream fl = new FileInputStream(ob);
        } catch (IOException ioe) { // UndCC_Violation
            ioe.printStackTrace();
        }

    }
}
