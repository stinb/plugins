import java.io.File;

public class Test01 {
  public static void main(String args[]) {
    File file = new File("file");

    file.delete();                  // UNDCC_Violation
    file.deleteOnExit();            // UNDCC_Violation
    file.mkdir();                   // UNDCC_Violation
    file.mkdirs();                  // UNDCC_Violation
    file.renameTo(file);            // UNDCC_Violation
    file.setLastModified(0);        // UNDCC_Violation
    file.setReadOnly();             // UNDCC_Violation
    file.setWritable(true, true);   // UNDCC_Violation
    file.setWritable(true);         // UNDCC_Violation
    file.setReadable(true, true);   // UNDCC_Violation
    file.setReadable(true);         // UNDCC_Violation
    file.setExecutable(true, true); // UNDCC_Violation
    file.setExecutable(true);       // UNDCC_Violation

    if (!file.delete()) {                  // UNDCC_Valid
      System.out.println("Operation failed");
    }
    if (!file.mkdir()) {                   // UNDCC_Valid
      System.out.println("Operation failed");
    }
    if (!file.mkdirs()) {                  // UNDCC_Valid
      System.out.println("Operation failed");
    }
    if (!file.renameTo(file)) {            // UNDCC_Valid
      System.out.println("Operation failed");
    }
    if (!file.setLastModified(0)) {        // UNDCC_Valid
      System.out.println("Operation failed");
    }
    if (!file.setReadOnly()) {             // UNDCC_Valid
      System.out.println("Operation failed");
    }
    if (!file.setWritable(true, true)) {   // UNDCC_Valid
      System.out.println("Operation failed");
    }
    if (!file.setWritable(true)) {         // UNDCC_Valid
      System.out.println("Operation failed");
    }
    if (!file.setReadable(true, true)) {   // UNDCC_Valid
      System.out.println("Operation failed");
    }
    if (!file.setReadable(true)) {         // UNDCC_Valid
      System.out.println("Operation failed");
    }
    if (!file.setExecutable(true, true)) { // UNDCC_Valid
      System.out.println("Operation failed");
    }
    if (!file.setExecutable(true)) {       // UNDCC_Valid
      System.out.println("Operation failed");
    }
  }
}


