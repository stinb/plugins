import java.io.Serializable;
import java.io.ObjectInputStream;
import java.io.IOException;

class ImmutableSer2 implements Serializable {
    private static final Integer s = new Integer(0);
    private Integer n = null; // UndCC_Valid - Immutable component

    public ImmutableSer2(Integer m) {
        n = new Integer(m); // Constructor performs defensive copying
    }

    private void readObject(ObjectInputStream ois) throws IOException, ClassNotFoundException {
        ois.defaultReadObject();
    }
}
