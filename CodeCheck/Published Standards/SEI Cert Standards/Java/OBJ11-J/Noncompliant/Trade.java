import java.io.IOException;

class Trade {
    private static Storage s; // UNDCC_Violation, uses a nonfinal static variable
    static {
        try {
            s = new Storage();
        } catch (IOException e) {
            /* Does not initialize s to a safe state */
        }
    }
    // ...
}