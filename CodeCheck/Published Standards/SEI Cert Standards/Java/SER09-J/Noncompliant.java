import java.io.Serializable;
import java.io.ObjectInputStream;
import java.io.IOException;

public class Noncompliant implements Serializable {
    private void readObject(final ObjectInputStream stream)
            throws IOException, ClassNotFoundException {
        overridableMethod(); // UNDCC_Violation, overridable method is called
        stream.defaultReadObject();
    }

    public void overridableMethod() {
        // ...
    }

}
