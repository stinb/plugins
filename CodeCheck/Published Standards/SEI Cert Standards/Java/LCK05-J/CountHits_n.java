/* This class is not thread-safe */
public final class CountHits_n {
    private static int counter;

    public void incrementCounter() {
        counter++;      //UndCC_Violation
    }
}
