import java.io.*;

public class Compliant2 {
  static final String FILEPATH = "/path/to/protected/file/fn.ext";
   
  private void privilegedMethod() throws FileNotFoundException {
    try {
      FileInputStream fis =
          (FileInputStream) AccessController.doPrivileged(
              new PrivilegedExceptionAction() {
          public FileInputStream run() throws FileNotFoundException {
            return new FileInputStream(FILEPATH);  // UndCC_Valid
          }
        }
      );
      // Do something with the file and then close it
    } catch (PrivilegedActionException e) {
      // Forward to handler and log
    }
  }
}
