import java.io.Serializable;
import java.util.Date;
import java.io.ObjectInputStream;
import java.io.IOException;

class MutableSer_n implements Serializable {
    private static final Date epoch = new Date(0);
    private Date date = null; // UndCC_Violation, Mutable component
    private String s = null;

    public MutableSer_n(Date d) {
        date = new Date(d.getTime()); // Constructor performs defensive copying
    }

    private void readObject(ObjectInputStream ois) throws IOException, ClassNotFoundException {
        ois.defaultReadObject();
        // Perform validation if necessary
    }
}
