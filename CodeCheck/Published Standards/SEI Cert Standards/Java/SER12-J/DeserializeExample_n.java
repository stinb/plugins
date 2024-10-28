import java.io.*;

class DeserializeExample_n {
    public static Object deserialize(byte[] buffer) throws IOException, ClassNotFoundException {
        Object ret = null;
        try (ByteArrayInputStream bais = new ByteArrayInputStream(buffer)) {
            try (ObjectInputStream ois = new ObjectInputStream(bais)) {
                ret = ois.readObject(); //UndCC_Violation
            }
        }
        return ret;
    }
}
