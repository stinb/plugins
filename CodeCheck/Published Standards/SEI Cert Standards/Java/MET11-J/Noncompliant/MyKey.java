import java.io.Serializable;
import java.util.Hashtable;
import java.io.IOException;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.io.FileInputStream;
import java.io.ObjectInputStream;

class MyKey implements Serializable {
    // Does not override hashCode()
}

class HashSer_3 {
    public static void main(String[] args)
            throws IOException, ClassNotFoundException {
        Hashtable<MyKey, String> ht = new Hashtable<MyKey, String>();   //UndCC_Violation
        MyKey key = new MyKey();
        ht.put(key, "Value");
        System.out.println("Entry: " + ht.get(key));
        // Retrieve using the key, works

        // Serialize the Hashtable object
        FileOutputStream fos = new FileOutputStream("hashdata.ser");
        ObjectOutputStream oos = new ObjectOutputStream(fos);
        oos.writeObject(ht);
        oos.close();

        // Deserialize the Hashtable object
        FileInputStream fis = new FileInputStream("hashdata.ser");
        ObjectInputStream ois = new ObjectInputStream(fis);
        Hashtable<MyKey, String> ht_in = (Hashtable<MyKey, String>) (ois.readObject());
        ois.close();

        if (ht_in.contains("Value"))
            // Check whether the object actually exists in the hash table
            System.out.println("Value was found in deserialized object.");

        if (ht_in.get(key) == null) // Gets printed
            System.out.println(
                    "Object was not found when retrieved using the key.");
    }
}
