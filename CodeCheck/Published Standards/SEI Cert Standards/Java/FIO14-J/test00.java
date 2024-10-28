import java.io.BufferedOutputStream;
import java.io.InputStream;
import java.io.IOException;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

public class CreateFile0 {
  public static void main(String[] args)
                          throws FileNotFoundException {
    final PrintStream out =
        new PrintStream(new BufferedOutputStream(
                        new FileOutputStream("foo.txt")));
    out.println("hello");
    Runtime.getRuntime().exit(1); // UndCC_Violation - output opened then exit
  }
}

public class CreateFile1 {
  public static void main(String[] args)
                          throws FileNotFoundException {
    final PrintStream out =
    new PrintStream(new BufferedOutputStream(
        new FileOutputStream("foo.txt")));
    try {
      out.println("hello");
    } finally {
      out.close();
    }
    Runtime.getRuntime().exit(1); // UndCC_Valid - output closed then exit
  }
}

public class CreateFile2 {
  public static void main(String[] args)
                          throws FileNotFoundException {
    final PrintStream out =
        new PrintStream(new BufferedOutputStream(
                        new FileOutputStream("foo.txt")));
    Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
        public void run() {
          out.close();
        }
    }));
    out.println("hello");
    Runtime.getRuntime().exit(1); // UndCC_Valid - output closed in exit hook
  }
}

public class CreateFile3 {
  public static void main(String[] args)
                          throws FileNotFoundException {
    final PrintStream out =
          new PrintStream(new BufferedOutputStream(
                          new FileOutputStream("foo.txt")));
    Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
        public void run() {
          out.close();
        }
    }));
    out.println("hello");
    Runtime.getRuntime().halt(1); // UndCC_Violation - output opened then halt
  }
}

public class InterceptExit {
  public static void main(String[] args)
                          throws FileNotFoundException {
    InputStream in = null;
    try {
      in = new FileInputStream("file"); // UndCC_Violation - input opened then user exit
      System.out.println("Regular code block");
      // Abrupt exit such as ctrl + c key pressed
      System.out.println("This never executes");
    } finally {
      if (in != null) {
        try {
          in.close();  // This never executes either
        } catch (IOException x) {
          // Handle error
        }
      }
    }
  }
}

public class Hook {
  public static void main(String[] args) {
    try {
      final InputStream in = new FileInputStream("file"); // UndCC_Valid - input closed in exit hook

      Runtime.getRuntime().addShutdownHook(new Thread() {
        public void run() {
          // Log shutdown and close all resources
          try {
            in.close();
          } catch (IOException x) {
            // Handle error
          }
        }
      });

      // ...
    } catch (IOException x) {
      // Handle error
    }
  }
}
