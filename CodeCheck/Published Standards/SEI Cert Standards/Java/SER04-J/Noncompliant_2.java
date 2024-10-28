import java.io.Serializable;
import java.nio.file.NoSuchFileException;
import java.io.ObjectOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.NoSuchElementException;

public final class Legalname implements Serializable {
  // Private internal state
  private String name;
  private static final String UNKNOWN = "UNKNOWN";

  void randomSecurityCheck() throws NoSuchFileException {
    // ...
  }

  void randomStringValidation(String newCC) throws NoSuchElementException {
    // ...
  }

  public Legalname() {
    try {
      randomSecurityCheck();
    } catch (NoSuchFileException exc) {
    }

    // Initialize name to default value
    name = UNKNOWN;
  }

  // Allows callers to retrieve internal state
  String getValue() {
    try {
      randomSecurityCheck();
    } catch (NoSuchFileException exc) {
    }

    return name;
  }

  // Allows callers to modify (private) internal state
  public void changeName(String newName) {
    if (name.equals(newName)) {
      // No change
      return;
    } else {
      try {
        randomSecurityCheck();
      } catch (NoSuchFileException exc) {
      }

      randomStringValidation(newName);
      name = newName;
    }
  }

  private void writeObject(ObjectOutputStream out) throws IOException { // UndCC_Violation
    out.writeObject(name);
  }

  private void readObject(ObjectInputStream in) throws IOException { // UndCC_Violation
    try {
      in.defaultReadObject();
    } catch (Exception exc) {
    }
    // If the deserialized name does not match the default value normally
    // created at construction time, duplicate the checks
    if (!UNKNOWN.equals(name)) {
      randomStringValidation(name);
    }
  }
}
