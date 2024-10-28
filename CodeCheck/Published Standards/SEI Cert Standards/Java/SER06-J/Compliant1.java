import java.io.Serializable;
import java.util.Date;
import java.io.ObjectInputStream;
import java.io.IOException;

class MutableSer1 implements Serializable {
    private static final Date epoch = new Date(0);
    private Date date = null; // // UndCC_Valid, Mutable component

    public MutableSer1(Date d) {
        date = new Date(d.getTime()); // Constructor performs defensive copying
    }

    private void readObject(ObjectInputStream ois) throws IOException, ClassNotFoundException {
        ObjectInputStream.GetField fields = ois.readFields();
        Date inDate = (Date) fields.get("date", epoch);
        // Defensively copy the mutable component
        date = new Date(inDate.getTime());
        // Perform validation if necessary
    }
}
