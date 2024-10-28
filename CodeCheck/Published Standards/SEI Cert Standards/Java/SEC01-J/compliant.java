import java.io.*;

public class Compliant {
  private void privilegedMethod(final String filename)
                                throws FileNotFoundException {
    final String cleanFilename;
    try {
      cleanFilename = cleanAFilenameAndPath(filename);
    } catch (exception e/* exception as per spec of cleanAFileNameAndPath */) {
      // Log or forward to handler as appropriate based on specification
      // of cleanAFilenameAndPath
    }
    try {
      FileInputStream fis =
          (FileInputStream) AccessController.doPrivileged(
            new PrivilegedExceptionAction() {
          public FileInputStream run() throws FileNotFoundException {
            return new FileInputStream(cleanFilename); // UndCC_Valid
          }
        }
      );
      // Do something with the file and then close it
    } catch (PrivilegedActionException e) {
      // Forward to handler
    }
  }
}
