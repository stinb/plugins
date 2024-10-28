import java.io.*;
import java.nio.file.*;
import java.security.*;

public class Noncompliant3 {
  private void privilegedMethod(final String filename)
                                throws FileNotFoundException {
    try {
      Path path = (Path) AccessController.doPrivileged(
        new PrivilegedExceptionAction() {
          public Path run() throws FileNotFoundException {
            return Files.createTempDirectory(filename); // UndCC_Violation
          }
        }
      );
      // Do something with the file and then close it
    } catch (PrivilegedActionException e) {
      // Forward to handler
    }
  }
}
