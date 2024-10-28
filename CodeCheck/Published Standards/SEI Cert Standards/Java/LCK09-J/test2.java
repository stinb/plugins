import java.util.concurrent.*;

public class Test2 {

    String input = "Nothing added yet!";
    LinkedBlockingQueue<String> buffer = new LinkedBlockingQueue<>();

    public synchronized String getFromBuffer() {
        System.out.println("Trying to get input from buffer.");
        try {
            input = buffer.take(); // UndCC_Violation
        } catch (InterruptedException ex) {}
        System.out.println("Got:" + input + "\n");
        return input;
    }

    public static void main(String[] args) throws InterruptedException {
        Test2 dl = new Test2();

        new Thread(() -> {
            dl.getFromBuffer();
        }).start();

        // Give new thread time to run.
        Thread.sleep(500);

        synchronized (dl) {
            String message = "Hello, world!";

            System.out.println("Adding: " + message);
            dl.buffer.put(message); // UndCC_Violation
            System.out.println("Added!\n");

            System.out.println("Message: " + dl.input);
        }
    }
}
