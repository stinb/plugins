import java.lang.*;
import java.util.zip.*;
import java.io.*;

public class Main
{

  static final int BUFFER = 512;
  static final long TOOBIG = 0x6400000; // Max size of unzipped data, 100MB
  static final int TOOMANY = 1024;      // Max number of files
  // ...

  private String validateFilename(String filename, String intendedDir)
  throws java.io.IOException {
    File f = new File(filename);
    String canonicalPath = f.getCanonicalPath();
    
    File iD = new File(intendedDir);
    String canonicalID = iD.getCanonicalPath();
    
    if (canonicalPath.startsWith(canonicalID)) {
      return canonicalPath;
    } else {
      throw new IllegalStateException("File is outside extraction target directory.");
    }
  }

  public final void unzip(String filename) throws java.io.IOException {
    FileInputStream fis = new FileInputStream(filename);
    ZipInputStream zis = new ZipInputStream(new BufferedInputStream(fis));
    ZipEntry entry;
    int entries = 0;
    long total = 0;
    try {
      while ((entry = zis.getNextEntry()) != null) {
        System.out.println("Extracting: " + entry);
        int count;
        byte data[] = new byte[BUFFER];
        // Write the files to the disk, but ensure that the filename is valid,
        // and that the file is not insanely big
        String name = validateFilename(entry.getName(), ".");
        if (entry.isDirectory()) {
          System.out.println("Creating directory " + name);
          new File(name).mkdir();
          continue;
        }
        FileOutputStream fos = new FileOutputStream(name);
        BufferedOutputStream dest = new BufferedOutputStream(fos, BUFFER);
        while (total + BUFFER <= TOOBIG && (count = zis.read(data, 0, BUFFER)) != -1) {
          dest.write(data, 0, count);
          total += count;
        }
        dest.flush();
        dest.close();
        zis.closeEntry();
        entries++;
        if (entries > TOOMANY) {
          throw new IllegalStateException("Too many files to unzip.");
        }
        if (total + BUFFER > TOOBIG) {
          throw new IllegalStateException("File being unzipped is too big.");
        }
      }
    } finally {
      zis.close();
    }
  }
}
