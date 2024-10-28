import java.io.Serializable;
import java.io.ObjectInputStream;
import java.io.IOException;

public class CodeExample implements Serializable {
    private void readObject(final ObjectInputStream stream)
    throws IOException, ClassNotFoundException {
        getRateOfInterest();          // UNDCC_Violation - overridable method is called
        stream.defaultReadObject();
    }

    public int getRateOfInterest() {
        return 8;
    }

}
