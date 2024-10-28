import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Lock;

// Class for send a message using Threads
class ThreadedSend_n extends Thread {
    Sender sender;
    final Lock lock = new ReentrantLock();
    final Condition notFull = lock.newCondition();

    public void run() {
        // Only one thread can send a message
        // at a time.
        synchronized (sender) {
            if (true) {
                try {
                    sender.wait(); // UndCC_Violation
                    notFull.await(); // UndCC_Violation
                } catch (InterruptedException ie) {
                    System.out.println(ie);
                }
            }
            // Proceed when condition holds
        }
    }
}

public class Noncompliant {

}
