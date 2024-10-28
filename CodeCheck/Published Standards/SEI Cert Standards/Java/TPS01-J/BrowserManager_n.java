import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.Callable;
import java.lang.reflect.Method;
import java.util.Collection;
import java.util.Collections;
import java.util.concurrent.Future;

public final class BrowserManager_n {
    private final ExecutorService pool = Executors.newFixedThreadPool(10);
    private final int numberOfTimes;
    private static AtomicInteger count = new AtomicInteger(); // count = 0

    public BrowserManager_n(int n) {
        numberOfTimes = n;
    }

    public void perUser() {
        methodInvoker(numberOfTimes, "perProfile");
        pool.shutdown();
    }

    public void perProfile() {
        methodInvoker(numberOfTimes, "perTab");
    }

    public void perTab() {
        methodInvoker(numberOfTimes, "doSomething");
    }

    public void doSomething() {
        System.out.println(count.getAndIncrement());
    }

    public void methodInvoker(int n, final String method) {
        final BrowserManager_n manager = this;
        Callable<Object> callable = new Callable<Object>() {
            @Override
            public Object call() throws Exception {
                Method meth = manager.getClass().getMethod(method);
                return meth.invoke(manager);
            }
        };

        Collection<Callable<Object>> collection = Collections.nCopies(n, callable);
        try {
            Collection<Future<Object>> futures = pool.invokeAll(collection);    //UndCC_Violation
        } catch (InterruptedException e) {
            // Forward to handler
            Thread.currentThread().interrupt(); // Reset interrupted status
        }
        // ...
    }

    public static void main(String[] args) {
        BrowserManager_n manager = new BrowserManager_n(5);
        manager.perUser();
    }
}
