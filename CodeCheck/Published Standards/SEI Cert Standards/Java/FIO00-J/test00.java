import java.io.*;
import java.nio.file.*;
import java.nio.file.attribute.*;
import java.util.*;



public class SharedDirectories {
  // Noncompliant Code Example
  public static void read0(String file) throws FileNotFoundException {
    // String file = /* Provided by user */;
    InputStream in = null;
    try {
      in = new FileInputStream(file); // UndCC_Violation
      // ...
    } finally {
      try {
        if (in !=null) { in.close();}
      } catch (IOException x) {
        // Handle error
      }
    }
  }



  // Noncompliant Code Example
  public static void read1(String filename) {
    // String filename = /* Provided by user */;
    Path path = new File(filename).toPath();
    try (InputStream in = Files.newInputStream(path)) { // UndCC_Violation
      // Read file
    } catch (IOException x) {
      // Handle error
    }
  }



  // Noncompliant Code Example (isRegularFile())
  public static void read2(String filename) {
    // String filename = /* Provided by user */
    Path path = new File(filename).toPath();
    try {
      BasicFileAttributes attr =
          Files.readAttributes(path, BasicFileAttributes.class);

      // Check
      if (!attr.isRegularFile()) {
        System.out.println("Not a regular file");
        return;
      }
      // Other necessary checks

      // Use
      try (InputStream in = Files.newInputStream(path)) { // UndCC_Violation
        // Read file
      }
    } catch (IOException x) {
      // Handle error
    }
  }



  // Noncompliant Code Example (NOFOLLOW_LINKS)
  public static void read3(String filename) {
    // String filename = /* Provided by user */;
    Path path = new File(filename).toPath();
    try {
      BasicFileAttributes attr = Files.readAttributes(
          path, BasicFileAttributes.class, LinkOption.NOFOLLOW_LINKS);

      // Check
      if (!attr.isRegularFile()) {
        System.out.println("Not a regular file");
        return;
      }
      // Other necessary checks

      // Use
      try (InputStream in = Files.newInputStream(path)) { // UndCC_Violation
        // Read file
      };
    } catch (IOException x) {
      // Handle error
    }
  }



  // Noncompliant Code Example (POSIX: Check-Use-Check)
  public static void read4(String filename) {
    // String filename = /* Provided by user */;
    Path path = new File(filename).toPath();
    try {
      BasicFileAttributes attr = Files.readAttributes(
          path, BasicFileAttributes.class, LinkOption.NOFOLLOW_LINKS);
      Object fileKey = attr.fileKey();

      // Check
      if (!attr.isRegularFile()) {
        System.out.println("Not a regular file");
        return;
      }
      // Other necessary checks

      // Use
      try (InputStream in = Files.newInputStream(path)) { // UndCC_Violation

        // Check
        BasicFileAttributes attr2 = Files.readAttributes(
            path, BasicFileAttributes.class, LinkOption.NOFOLLOW_LINKS
        );
        Object fileKey2 = attr2.fileKey();
        if (!fileKey.equals(fileKey2)) {
          System.out.println("File has been tampered with");
        }

        // Read file
      };
    } catch (IOException x) {
      // Handle error
    }
  }



  // Compliant Solution (POSIX: Secure Directory)
  public static boolean isInSecureDir(Path file) {
    return isInSecureDir(file, null);
  }
  public static boolean isInSecureDir(Path file, UserPrincipal user) {
    return isInSecureDir(file, user, 5);
  }

  /**
  * Indicates whether file lives in a secure directory relative
  * to the program's user
  * @param file Path to test
  * @param user User to test. If null, defaults to current user
  * @param symlinkDepth Number of symbolic links allowed
  * @return true if file's directory is secure.
  */
  public static boolean isInSecureDir(Path file, UserPrincipal user,
                                      int symlinkDepth) {
    if (!file.isAbsolute()) {
      file = file.toAbsolutePath();
    } if (symlinkDepth <=0) {
      // Too many levels of symbolic links
      return false;
    }

    // Get UserPrincipal for specified user and superuser
    FileSystem fileSystem =
        Paths.get(file.getRoot().toString()).getFileSystem();
    UserPrincipalLookupService upls =
        fileSystem.getUserPrincipalLookupService();
    UserPrincipal root = null;
    try {
      root = upls.lookupPrincipalByName("root");
      if (user == null) {
        user = upls.lookupPrincipalByName(System.getProperty("user.name"));
      }
      if (root == null || user == null) {
        return false;
      }
    } catch (IOException x) {
      return false;
    }

    // If any parent dirs (from root on down) are not secure,
    // dir is not secure
    for (int i = 1; i <= file.getNameCount(); i++) {
      Path partialPath = Paths.get(file.getRoot().toString(),
                                  file.subpath(0, i).toString());

      try {
        if (Files.isSymbolicLink(partialPath)) {
          if (!isInSecureDir(Files.readSymbolicLink(partialPath),
                            user, symlinkDepth - 1)) {
            // Symbolic link, linked-to dir not secure
            return false;
          }
        } else {
          UserPrincipal owner = Files.getOwner(partialPath);
          if (!user.equals(owner) && !root.equals(owner)) {
            // dir owned by someone else, not secure
            return false;
          }
          PosixFileAttributes attr =
              Files.readAttributes(partialPath, PosixFileAttributes.class);
          Set<PosixFilePermission> perms = attr.permissions();
          if (perms.contains(PosixFilePermission.GROUP_WRITE) ||
              perms.contains(PosixFilePermission.OTHERS_WRITE)) {
            // Someone else can write files, not secure
            return false;
          }
        }
      } catch (IOException x) {
        return false;
      }
    }

    return true;
  }

  public static void read5(String filename) {
    // String filename = /* Provided by user */;
    Path path = new File(filename).toPath();
    try {
      if (!isInSecureDir(path)) {
        System.out.println("File not in secure directory");
        return;
      }

      BasicFileAttributes attr = Files.readAttributes(
          path, BasicFileAttributes.class, LinkOption.NOFOLLOW_LINKS);

      // Check
      if (!attr.isRegularFile()) {
        System.out.println("Not a regular file");
        return;
      }
      // Other necessary checks

      try (InputStream in = Files.newInputStream(path)) { // UndCC_Valid
        // Read file
      }
    } catch (IOException x) {
      // Handle error
    }
  }
}
