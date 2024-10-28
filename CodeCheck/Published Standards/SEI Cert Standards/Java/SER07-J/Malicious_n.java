import java.io.ByteArrayOutputStream;
import java.io.ObjectOutputStream;
import java.io.ByteArrayInputStream;
import java.io.ObjectInputStream;

class Malicious_n {
    public static void main(String[] args) {
        NumberData_n sc = (NumberData_n) deepCopy(NumberData_n.getInstance());
        // Prints false; indicates new instance
        System.out.println(sc == NumberData_n.getInstance());
        System.out.println("Balance = " + sc.printData());
    }

    // This method should not be used in production code
    public static Object deepCopy(Object obj) {
        try {
            ByteArrayOutputStream bos = new ByteArrayOutputStream();
            new ObjectOutputStream(bos).writeObject(obj);
            ByteArrayInputStream bin = new ByteArrayInputStream(bos.toByteArray());
            return new ObjectInputStream(bin).readObject();
        } catch (Exception e) {
            throw new IllegalArgumentException(e);
        }
    }
}