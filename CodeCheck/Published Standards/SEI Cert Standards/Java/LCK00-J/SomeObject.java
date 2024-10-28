public class SomeObject {
    private final Object lock = new Object(); // private final lock object

    public void changeValue() {
        synchronized (lock) { // UndCC_Valid, Locks on the private Object
            // ...
        }
    }
}