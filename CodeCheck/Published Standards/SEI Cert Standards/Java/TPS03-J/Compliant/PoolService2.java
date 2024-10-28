import java.util.concurrent.ThreadFactory;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

final class PoolService2 {
    private static final ThreadFactory factory = new ExceptionThreadFactory(new MyExceptionHandler());
    private static final ExecutorService pool = Executors.newFixedThreadPool(10, factory);

    public void doSomething() {
        pool.execute(new Task()); // UndCC, Valid, uncaughtException overridden on factory, Task is a runnable class
    }

    public static class ExceptionThreadFactory implements ThreadFactory {
        private static final ThreadFactory defaultFactory = Executors.defaultThreadFactory();
        private final Thread.UncaughtExceptionHandler handler;

        public ExceptionThreadFactory(
                Thread.UncaughtExceptionHandler handler) {
            this.handler = handler;
        }

        @Override
        public Thread newThread(Runnable run) {
            Thread thread = defaultFactory.newThread(run);
            thread.setUncaughtExceptionHandler(handler);
            return thread;
        }
    }

    public static class MyExceptionHandler extends ExceptionReporter
            implements Thread.UncaughtExceptionHandler {
        // ...

        @Override
        public void uncaughtException(Thread thread, Throwable t) {
            // Recovery or logging code
        }
    }
}
