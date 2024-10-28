import java.io.FileWriter;
public class Test {
  public static void main(String[] args) {
    FileOutputStream fout = new FileOutputStream(File file); // UNDCC_Violation
    
    Writer out = new FileWriter("file"); // UNDCC_Violation

    File file1 = new File("file"); // UNDCC_Valid
    
    boolean exists = file.exists();
    if (exists == true) {

      // In this method the file permissions are able to be changed
      file.setExecutable(true);
      file.setReadable(true);
      file.setWritable(false);

      System.out.println("Executable: "
                         + file.canExecute());
      System.out.println("Readable: "
                         + file.canRead());
      System.out.println("Writable: "
                         + file.canWrite());
    }  else {
      System.out.println("File not found.");
    }
  }
}

