import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;

final class PoolService_n {
    private final ExecutorService pool = Executors.newFixedThreadPool(10);

    public void doSomething() {
        pool.execute(new Task_n());     //UndCC_Violation
    }
}
