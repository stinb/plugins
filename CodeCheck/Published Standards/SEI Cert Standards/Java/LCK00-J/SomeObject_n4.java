public class SomeObject_n4 {
    public final Object lock = new Object();

    public void changeValue() {
        synchronized (lock) { // UndCC_Violation, public lock object
            // ...
        }
    }
}