import java.io.InputStream;
import java.io.IOException;
import java.io.EOFException;

public class Noncompliant_2 {
    public static String readBytes_4(InputStream in) throws IOException {
        byte[] data = new byte[1024];
        int offset = 0;
        if (in.read(data, offset, data.length - offset) != -1) { // UndCC_Violation
            throw new EOFException();
        }
        return new String(data, "UTF-8");
    }

}
