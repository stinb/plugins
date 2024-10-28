import java.util.concurrent.Executor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.ArrayBlockingQueue;

public final class DiaryPool2 {
    final int numOfThreads = 2; // Maximum number of threads allowed in pool
    final Executor exec;
    final Diary diary;

    DiaryPool2() {
        exec = new CustomThreadPoolExecutor(numOfThreads, numOfThreads,
                10, TimeUnit.SECONDS, new ArrayBlockingQueue<Runnable>(10));
        diary = new Diary();
    }

    public void doSomething1() {
        exec.execute(new Runnable() {
            @Override
            public void run() {
                Diary.setDay(Day.FRIDAY); // UndCC_Valid, reinitialized using beforeExecute
                diary.threadSpecificTask();
            }
        });
    }

    // ...
}