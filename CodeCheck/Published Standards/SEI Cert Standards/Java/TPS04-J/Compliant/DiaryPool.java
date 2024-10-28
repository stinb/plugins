import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

public final class DiaryPool {
    final int numOfThreads = 2; // Maximum number of threads allowed in pool
    final Executor exec;
    final Diary diary;

    DiaryPool() {
        exec = (Executor) Executors.newFixedThreadPool(numOfThreads);
        diary = new Diary();
    }

    public void doSomething1() {
        exec.execute(new Runnable() {
            @Override
            public void run() {
                try {
                    Diary.setDay(Day.FRIDAY);   //UndCC_Valid, reinitialized using removeDay
                    diary.threadSpecificTask();
                } finally {
                    Diary.removeDay(); // Diary.setDay(Day.MONDAY)
                                       // can also be used
                }
            }
        });
    }

    // ...
}
