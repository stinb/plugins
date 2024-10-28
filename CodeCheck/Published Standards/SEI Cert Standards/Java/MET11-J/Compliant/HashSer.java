import java.io.IOException;
import java.util.Hashtable;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.io.FileInputStream;
import java.io.ObjectInputStream;
import java.lang.Integer;

class HashSer {
    public static void main(String[] args)
            throws IOException, ClassNotFoundException {
        Hashtable<Integer, String> ht = new Hashtable<Integer, String>();       //UndCC_Valid
        ht.put(new Integer(1), "Value");
        System.out.println("Entry: " + ht.get(1)); // Retrieve using the key

        // Serialize the Hashtable object
        FileOutputStream fos = new FileOutputStream("hashdata.ser");
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        oos.writeObject(ht);
        oos.close();

        // Deserialize the Hashtable object
        FileInputStream fis = new FileInputStream("hashdata.ser");
        ObjectInputStream ois = new ObjectInputStream(fis);
        Hashtable<Integer, String> ht_in = (Hashtable<Integer, String>) (ois.readObject());
        ois.close();

        if (ht_in.contains("Value"))
            // Check whether the object actually exists in the Hashtable
            System.out.println("Value was found in deserialized object.");

        if (ht_in.get(1) == null) // Not printed
            System.out.println(
                    "Object was not found when retrieved using the key.");
    }
}
