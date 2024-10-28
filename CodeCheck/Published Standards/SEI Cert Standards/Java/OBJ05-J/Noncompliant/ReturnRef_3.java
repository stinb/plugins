// Custom

import java.util.Hashtable;

class ReturnRef_3 {
    private Hashtable<Integer,String> ht = new Hashtable<Integer,String>();

    private Hashtable<Integer, String> getValues() {
        return (Hashtable<Integer, String>) ht; // UndCC_Violation
    }

    public static void main(String[] args) {
        ReturnRef_3 rr = new ReturnRef_3();
        System.out.println(rr.getValues());
        Hashtable<Integer, String> ht1 = rr.getValues();
        ht1.put(1, "123-45-6666");
        System.out.println(rr.getValues());
    }
}
