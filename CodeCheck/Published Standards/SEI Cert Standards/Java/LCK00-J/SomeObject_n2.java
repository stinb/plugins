
public class SomeObject_n2 {
    public Object lock = new Object();

    public void changeValue() {
        synchronized (lock) { // UndCC_Violation, public non-final lock object
            // ...
        }
    }
}