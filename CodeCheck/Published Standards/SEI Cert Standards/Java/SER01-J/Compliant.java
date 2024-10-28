import java.io.Serializable;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.IOException;

public class Compliant implements Serializable {
    private void writeObject(final ObjectOutputStream stream)  /* UndCC_Valid */
            throws IOException {
        stream.defaultWriteObject();
    }

    private void readObject(final ObjectInputStream stream)    /* UndCC_Valid */
            throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
    }
}
