import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.BlockingQueue;

class CustomThreadPoolExecutor extends ThreadPoolExecutor {
    public CustomThreadPoolExecutor(int corePoolSize,
            int maximumPoolSize, long keepAliveTime,
            TimeUnit unit, BlockingQueue<Runnable> workQueue) {
        super(corePoolSize, maximumPoolSize, keepAliveTime,
                unit, workQueue);
    }

    @Override
    public void beforeExecute(Thread t, Runnable r) {
        if (t == null || r == null) {
            throw new NullPointerException();
        }
        Diary.setDay(Day.MONDAY);
        super.beforeExecute(t, r);
    }
}
