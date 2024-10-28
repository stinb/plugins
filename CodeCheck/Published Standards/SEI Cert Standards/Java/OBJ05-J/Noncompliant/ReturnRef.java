// SEI CERT C++

import java.util.Hashtable;

class ReturnRef_2 {
    // Internal state, may contain sensitive data
    private Hashtable<Integer, String> ht = new Hashtable<Integer, String>();

    private ReturnRef_2() {
        ht.put(1, "123-45-6666");
    }

    public Hashtable<Integer, String> getValues() {
        return ht; // UNDCC_Violation
    }

    public static void main(String[] args) {
        ReturnRef_2 rr = new ReturnRef_2();
        Hashtable<Integer, String> ht1 = rr.getValues(); // Prints sensitive data 123-45-6666
        ht1.remove(1); // Untrusted caller can remove entries
        Hashtable<Integer, String> ht2 = rr.getValues(); // Now prints null; original entry is removed
    }
}
