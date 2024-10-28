import java.io.FileNotFoundException;
import java.io.FileInputStream;
import java.io.File;

public class EX_1 {
    public void myMethod() throws MyProgramException {
        File ob;
        // ...
        try {
            ob = new File("/aayush/home");
            FileInputStream fl = new FileInputStream(ob);
            // Requested file does not exist
            // User is unable to supply the file name
        } catch (FileNotFoundException e) {
            throw new MyProgramException(e); // UndCC_Valid, ERR00-J-EX1
        }
        // ...
    }

}
