import java.io.Serializable;
import java.io.ObjectInputStream;
import java.io.IOException;

public class Compliant implements Serializable {
    private void readObject(final ObjectInputStream stream)
            throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
    }

    public void overridableMethod() {
        // ...
    }

}
