import java.io.Serializable;
import java.lang.runtime.ObjectMethods;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.IOException;

public class Noncompliant implements Serializable {
    private final long serialVersionUID = 123456789;

    private Noncompliant() {
        // Initialize
    }

    public static void writeObject(final ObjectOutputStream stream) // UNDCC_Violation, serialization method declared
                                                                    // private and static
            throws IOException {
        stream.defaultWriteObject();
    }

    public static void readObject(final ObjectInputStream stream)  // UNDCC_Violation, serialization method declared
                                                                   // private and static
            throws IOException, ClassNotFoundException {
        stream.defaultReadObject();
    }

}
