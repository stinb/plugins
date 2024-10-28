public class SomeObject2 {
    private static final Object lock = new Object();

    public static void changeValue() {
        synchronized (lock) { // UndCC_Valid, Locks on the private Object
            // ...
        }
    }
}