import java.io.InputStream;
import java.io.IOException;
import java.io.EOFException;

public class Noncompliant {
    public static String readBytes_3(InputStream in) throws IOException {
        byte[] data = new byte[1024];
        if (in.read(data) == -1) { // UndCC_Violation
            throw new EOFException();
        }
        return new String(data, "UTF-8");
    }
}
