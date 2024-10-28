import java.util.concurrent.ExecutorService;
import java.io.IOException;
import java.util.concurrent.Executors;
import java.util.List;

public final class PoolService_n {
    private final ExecutorService pool;

    public PoolService_n(int poolSize) {
        pool = Executors.newFixedThreadPool(poolSize);
    }

    public void doSomething() throws InterruptedException, IOException {
        pool.submit(new SocketReader_n("somehost", 8080)); // UndCC_Violation
        // ...
        List<Runnable> awaitingTasks = pool.shutdownNow();
    }

    public static void main(String[] args)
            throws InterruptedException, IOException {
        PoolService_n service = new PoolService_n(5);
        service.doSomething();
    }
}
