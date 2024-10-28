import java.io.Serializable;
import java.lang.runtime.ObjectMethods;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.IOException;

public class Noncompliant2 implements Serializable {
    private final long serialVersionUID = 123456789;

    private Noncompliant2() {
        // Initialize
    }

    protected static Object readResolve() { // UNDCC_Violation, serialization method declared static
        Noncompliant2 obj = new Noncompliant2();
        return obj;
    }

    protected static Object writeReplace() { // UNDCC_Violation, serialization method declared static
        Noncompliant2 obj = new Noncompliant2();
        return obj;
    }

}
