public class SomeObject_n3 {
    private volatile Object lock = new Object();

    public void changeValue() {
        synchronized (lock) { // UndCC_Violation, non-final lock object
            // ...
        }
    }

    public void setLock(Object lockValue) {
        lock = lockValue;
    }
}