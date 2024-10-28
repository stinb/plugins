import java.io.Serializable;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.IOException;

public class Compliant2 implements Serializable {
    protected Object readResolve() {        /* UndCC_Valid */
        Compliant2 obj = new Compliant2();
        return obj;
    }

    protected Object writeReplace() {       /* UndCC_Valid */
        Compliant2 obj = new Compliant2();
        return obj;
    }

}
