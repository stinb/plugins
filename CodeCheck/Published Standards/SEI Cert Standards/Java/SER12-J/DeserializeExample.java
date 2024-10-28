import java.util.Set;
import java.io.IOException;
import java.util.HashSet;
import java.util.Arrays;
import java.io.ByteArrayInputStream;

class DeserializeExample {
    private static Object deserialize(byte[] buffer) throws IOException, ClassNotFoundException {
        Object ret = null;
        Set whitelist = new HashSet<String>(Arrays.asList(new String[] { "GoodClass1", "GoodClass2" }));
        try (ByteArrayInputStream bais = new ByteArrayInputStream(buffer)) {
            try (WhitelistedObjectInputStream ois = new WhitelistedObjectInputStream(bais, whitelist)) {
                ret = ois.readObject(); //UndCC_Valid
            }
        }
        return ret;
    }
}
