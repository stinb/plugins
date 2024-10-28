import java.io.*;

class ExceptionExample1 {
  public static void main(String[] args) {
    try {
      FileInputStream fis =
        new FileInputStream(System.getenv("APPDATA") + args[0]); // UndCC_Violation
    } catch (FileNotFoundException e) {
      System.out.println(e);
    }
  }
}
