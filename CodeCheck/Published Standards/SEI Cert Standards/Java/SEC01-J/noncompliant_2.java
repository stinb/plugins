import java.io.*;
import java.nio.file.*;
import java.security.*;

public class Noncompliant2 {
  private void privilegedMethod(final String filename)
                                throws IOException {
    try {
      InputStream fis = (InputStream) AccessController.doPrivileged(
        new PrivilegedExceptionAction() {
          public InputStream run() throws IOException {
            Path path = new File(filename).toPath(); // UndCC_Violation
            return Files.newInputStream(path);
          }
        }
      );
      // Do something with the file and then close it
    } catch (PrivilegedActionException e) {
      // Forward to handler
    }
  }
}
