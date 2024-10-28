import java.util.concurrent.ExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.ArrayBlockingQueue;

final class PoolService {
    ExecutorService pool = new CustomThreadPoolExecutor(10, 10, 10, TimeUnit.SECONDS,
            new ArrayBlockingQueue<Runnable>(10));

    public void doSomething() {
        pool.execute(new Task());       //UndCC_Valid, afterExecute method overridden
    }
}
