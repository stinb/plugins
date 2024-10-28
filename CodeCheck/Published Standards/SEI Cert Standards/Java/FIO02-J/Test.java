import java.io.*;
import java.nio.file.*;
public class Test {
  public static void main(String args[]) {
    File file = new File(args[0]);
    file.delete(); // UNDCC_Violation
    File file1 = new File("file");
    if (!file1.delete()) { // UNDCC_Valid
      System.out.println("Deletion failed");
    }

    Path file2 = new File(args[0]).toPath();
    try {
      Files.delete(file2); // UNDCC_Valid
    } catch (IOException x) {
      System.out.println("Deletion failed");
      // Handle error
    }
  }
}

