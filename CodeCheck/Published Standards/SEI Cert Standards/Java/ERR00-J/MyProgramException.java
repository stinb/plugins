import java.io.FileNotFoundException;
import java.lang.Exception;

public class MyProgramException extends Exception {

    public MyProgramException(FileNotFoundException e) {
    }

}
