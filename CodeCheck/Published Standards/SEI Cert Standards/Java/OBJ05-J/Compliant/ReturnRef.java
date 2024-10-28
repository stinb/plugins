// SEI CERT C++

import java.util.Hashtable;

class ReturnRef {
    // ...
    private Hashtable<Integer, String> getValues() {
        return (Hashtable<Integer, String>) ht.clone(); // Shallow copy
    }

    public static void main(String[] args) {
        ReturnRef rr = new ReturnRef();
        // Prints nonsensitive data
        Hashtable<Integer, String> ht1 = rr.getValues();
        // Untrusted caller can only modify copy
        ht1.remove(1);
        // Prints nonsensitive data
        Hashtable<Integer, String> ht2 = rr.getValues();
    }
}
