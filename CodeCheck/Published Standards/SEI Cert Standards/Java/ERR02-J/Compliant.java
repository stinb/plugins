import java.util.logging.Logger;
import java.util.logging.Level;

public class Compliant {

    public static void main(String[] args) {
        Logger logger = Logger.getLogger("");

        try {
            // ...
        } catch (SecurityException se) {
            logger.log(Level.SEVERE, se.getMessage()); // UndCC_Valid
            // Recover from exception
        }
    }

}
