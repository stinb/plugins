import java.io.*;
import java.security.*;
import java.util.logging.*;

class ExceptionExample0 {
  public static void main(String[] args)
      throws FileNotFoundException, IOException, SecurityIOException {
    // Noncompliant Code Example (Leaks from Exception Message and Type)
    // Linux stores a user's home directory path in
    // the environment variable $HOME, Windows in %APPDATA%
    FileInputStream fis0 =
        new FileInputStream(System.getenv("APPDATA") + args[0]); // UndCC_Violation



    // Noncompliant Code Example (Wrapping and Rethrowing Sensitive Exception)
    try {
      FileInputStream fis1 =
        new FileInputStream(System.getenv("APPDATA") + args[0]); // UndCC_Violation
    } catch (FileNotFoundException e) {
      // Log the exception
      throw new IOException("Unable to retrieve file", e);
    }



    // Noncompliant Code Example (Sanitized Exception)
    try {
      FileInputStream fis2 =
          new FileInputStream(System.getenv("APPDATA") + args[0]); // UndCC_Violation
    } catch (FileNotFoundException e) {
      // Log the exception
      throw new SecurityIOException();
    }



    // Compliant Solution (Security Policy)
    File file = null;
    try {
      file = new File(System.getenv("APPDATA") + // UndCC_Valid
             args[0]).getCanonicalFile();
      if (!file.getPath().startsWith("c:\\homepath")) {
        System.out.println("Invalid file");
        return;
      }
    } catch (IOException x) {
      System.out.println("Invalid file");
      return;
    }

    try {
      FileInputStream fis3 = new FileInputStream(file); // UndCC_Valid
    } catch (FileNotFoundException x) {
      System.out.println("Invalid file");
      return;
    }



    // Compliant Solution (Restricted Input)
    FileInputStream fis4 = null;
    try {
      switch(Integer.valueOf(args[0])) {
        case 1:
          fis4 = new FileInputStream("c:\\homepath\\file1"); // UndCC_Valid
          break;
        case 2:
          fis4 = new FileInputStream("c:\\homepath\\file2"); // UndCC_Valid
          break;
        //...
        default:
          System.out.println("Invalid option");
          break;
      }
    } catch (Throwable t) {
      MyExceptionReporter.report(t); // Sanitize
    }
  }
}

class SensitiveException1 extends Exception {/* ... */};
class SensitiveException2 extends Exception {/* ... */};
class FilteredSensitiveException extends Exception {
  public FilteredSensitiveException(Throwable t) {/* ... */}
};

class MyExceptionReporter {
  private static final Logger logger =
      Logger.getLogger("com.organization.Log");

  public static void report(Throwable t) {
    t = filter(t);
    if (t != null) {
      logger.log(Level.FINEST, "Loggable exception occurred", t);
    }
  }

  public static Exception filter(Throwable t) {
    if (t instanceof SensitiveException1) {
      // Too sensitive, return nothing (so that no logging happens)
      return null;
    } else if (t instanceof SensitiveException2) {
      // Return a default insensitive exception instead
      return new FilteredSensitiveException(t);
    }
    // ...
    // Return for reporting to the user
    return new Exception(t);
  }
}

class SecurityIOException extends IOException {/* ... */};
