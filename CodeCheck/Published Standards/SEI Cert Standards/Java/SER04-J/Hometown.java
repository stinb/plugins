import java.io.Serializable;
import java.nio.file.AccessDeniedException;
import java.io.ObjectOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.InputMismatchException;

public final class Hometown implements Serializable {
    // Private internal state
    private String town;
    private static final String UNKNOWN = "UNKNOWN";

    void performSecurityManagerCheck() throws AccessDeniedException {
        // ...
    }

    void validateInput(String newCC) throws InputMismatchException {
        // ...
    }

    public Hometown() {
        try {
            performSecurityManagerCheck();

        } catch (AccessDeniedException exc) {

        }

        // Initialize town to default value
        town = UNKNOWN;
    }

    // Allows callers to retrieve internal state
    String getValue() {
        try {
            performSecurityManagerCheck();
        } catch (AccessDeniedException exc) {
        }

        return town;
    }

    // Allows callers to modify (private) internal state
    public void changeTown(String newTown) {
        if (town.equals(newTown)) {
            // No change
            return;
        } else {
            try {
                performSecurityManagerCheck();
            } catch (AccessDeniedException exc) {
            }

            validateInput(newTown);
            town = newTown;
        }
    }
    // ... All methods the same except the following:

    // writeObject() correctly enforces checks during serialization
    private void writeObject(ObjectOutputStream out) throws IOException {
        performSecurityManagerCheck();
        out.writeObject(town);
    }

    // readObject() correctly enforces checks during deserialization
    private void readObject(ObjectInputStream in) throws IOException {
        try {
            in.defaultReadObject();
        } catch (Exception exc) {
        }
        // If the deserialized name does not match the default value normally
        // created at construction time, duplicate the checks
        if (!UNKNOWN.equals(town)) {
            performSecurityManagerCheck();
            validateInput(town);
        }
    }
}