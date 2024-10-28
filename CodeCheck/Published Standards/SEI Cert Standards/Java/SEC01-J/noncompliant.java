import java.io.*;

public class Noncompliant {
  private void privilegedMethod(final String filename)
                                throws FileNotFoundException {
    try {
      FileInputStream fis =
          (FileInputStream) AccessController.doPrivileged(
            new PrivilegedExceptionAction() {
          public FileInputStream run() throws FileNotFoundException {
            return new FileInputStream(filename);  // UndCC_Violation
          }
        }
      );
      // Do something with the file and then close it
    } catch (PrivilegedActionException e) {
      // Forward to handler
    }
  }
}
