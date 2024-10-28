import java.io.InputStream;
import java.io.IOException;
import java.io.FileInputStream;
import java.io.DataInputStream;

public class Compliant {
    public static String readBytes(InputStream in) throws IOException {
        int offset = 0;
        int bytesRead = 0;
        byte[] data = new byte[1024];
        while ((bytesRead = in.read(data, offset, data.length - offset)) != -1) { // UndCC_Valid
            offset += bytesRead;
            if (offset >= data.length) {
                break;
            }
        }
        String str = new String(data, 0, offset, "UTF-8");
        return str;
    }

    public static String readBytes_2(FileInputStream fis)
            throws IOException {
        byte[] data = new byte[1024];
        DataInputStream dis = new DataInputStream(fis);
        dis.readFully(data); // UndCC_Valid
        String str = new String(data, "UTF-8");
        return str;
    }

}
