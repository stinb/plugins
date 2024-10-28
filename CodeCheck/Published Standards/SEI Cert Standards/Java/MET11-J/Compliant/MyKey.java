import java.io.Serializable;
import java.util.Hashtable;
import java.io.IOException;
import java.io.FileOutputStream;
import java.io.ObjectOutputStream;
import java.io.FileInputStream;
import java.io.ObjectInputStream;

class MyKey_2 implements Serializable {
    int sample;

    @Override
    public int hashCode() {
        return sample;
    }

}

class HashSer_4 {
    public static void main(String[] args)
            throws IOException, ClassNotFoundException {
        Hashtable<MyKey_2, String> ht = new Hashtable<MyKey_2, String>();       //UndCC_Valid
        MyKey_2 key = new MyKey_2();
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
        Hashtable<MyKey_2, String> ht_in = (Hashtable<MyKey_2, String>) (ois.readObject());
        ois.close();

        if (ht_in.contains("Value"))
            // Check whether the object actually exists in the hash table
            System.out.println("Value was found in deserialized object.");

        if (ht_in.get(key) == null) // Gets printed
            System.out.println(
                    "Object was not found when retrieved using the key.");
    }
}
