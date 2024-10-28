import java.util.*;
import java.io.*;
import java.nio.file.*;
import java.nio.charset.*;
public class Test {
  
  public static void test() throws IOException{
    File f = new File("tempnam.tmp"); // UNDCC_Violation
    if (f.exists()) {
      System.out.println("This file already exists");
      return;
    }
    
    FileOutputStream fop = null;
    try {
      fop = new FileOutputStream(f);
      String str = "Data";
      fop.write(str.getBytes());
    } finally {
      if (fop != null) {
        try {
          fop.close();
        } catch (IOException x) {
          // Handle error
        }
      }
    }
  }
  public static void test1() throws IOException{
    File f = File.createTempFile("tempnam",".tmp"); // UNDCC_Violation
    FileOutputStream fop = null;
    try {
      fop = new FileOutputStream(f);
      String str = "Data";
      fop.write(str.getBytes());
      fop.flush();
    } finally {
      // Stream/file still open; file will
      // not be deleted on Windows systems
      f.deleteOnExit(); // Delete the file when the JVM terminates
      
      if (fop != null) {
        try {
          fop.close();
        } catch (IOException x) {
          // Handle error
        }
      }
    }
  }
  public static void test2() {
    Path tempFile = null;
    try {
      tempFile = Files.createTempFile("tempnam", ".tmp"); // UNDCC_Valid
      try (BufferedWriter writer =
               Files.newBufferedWriter(tempFile, Charset.forName("UTF8"),
                                       StandardOpenOption.DELETE_ON_CLOSE)) {
        // Write to file
      }
      System.out.println("Temporary file write done, file erased");
    } catch (FileAlreadyExistsException x) {
      System.err.println("File exists: " + tempFile);
    } catch (IOException x) {
      // Some other sort of failure, such as permissions.
      System.err.println("Error creating temporary file: " + x);
    }
  }
  
}

