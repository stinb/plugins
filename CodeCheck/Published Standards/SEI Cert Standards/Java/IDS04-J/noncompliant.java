import java.lang.*;
import java.util.zip.*;
import java.io.*;

public class Main2
{

  static final int BUFFER = 512;
  static final int TOOBIG = 0x6400000; // 100MB
  // ...

  public final void unzip(String filename) throws java.io.IOException{
    FileInputStream fis = new FileInputStream(filename);
    ZipInputStream zis = new ZipInputStream(new BufferedInputStream(fis));
    ZipEntry entry;
    try {
      while ((entry = zis.getNextEntry()) != null) {
        System.out.println("Extracting: " + entry);
        int count;
        byte data[] = new byte[BUFFER];
        // Write the files to the disk, but only if the file is not insanely big
        if (entry.getSize() > TOOBIG ) { // UndCC_Violation
          throw new IllegalStateException("File to be unzipped is huge.");
        }
        if (entry.getSize() == -1) {     // UndCC_Violation
          throw new IllegalStateException("File to be unzipped might be huge.");
        }
        FileOutputStream fos = new FileOutputStream(entry.getName());
        BufferedOutputStream dest = new BufferedOutputStream(fos, BUFFER);
        while ((count = zis.read(data, 0, BUFFER)) != -1) {
          dest.write(data, 0, count);
        }
        dest.flush();
        dest.close();
        zis.closeEntry();
      }
    } finally {
      zis.close();
    }
  }
}
