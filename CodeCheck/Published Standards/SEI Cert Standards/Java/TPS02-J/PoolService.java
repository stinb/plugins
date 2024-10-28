import java.util.concurrent.ExecutorService;
import java.io.IOException;
import java.util.concurrent.Executors;
import java.util.List;

public final class PoolService {
    private final ExecutorService pool;

    public PoolService(int poolSize) {
        pool = Executors.newFixedThreadPool(poolSize);
    }

    public void doSomething() throws InterruptedException, IOException {
        pool.submit(new SocketReader("somehost", 8080)); // UndCC_Valid
        // ...
        List<Runnable> awaitingTasks = pool.shutdownNow();
    }

    public static void main(String[] args)
            throws InterruptedException, IOException {
        PoolService service = new PoolService(5);
        service.doSomething();
    }
}