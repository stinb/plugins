public class Test 
{

  public void readSensitiveFile01() {
    try {
      SecurityManager sm = System.getSecurityManager();
      if (sm != null) {  // Check for permission to read file
        sm.checkRead("/temp/tempFile");                         // UndCC_Violation
        sm.getSecurityContext();
      }
      // Access the file
    } catch (SecurityException se) {
      // Log exception
    }
  }
  
  protected void readSensitiveFile01() {
    try {
      SecurityManager sm = System.getSecurityManager();
      if (sm != null) {  // Check for permission to read file
        sm.checkLink("/temp/tempFile");                         // UndCC_Violation
        sm.getSecurityContext();
      }
      // Access the file
    } catch (SecurityException se) {
      // Log exception
    }
  }
  
  
  
  public final void readSensitiveFile02() {
    try {
      SecurityManager sm = System.getSecurityManager();
      if (sm != null) {  // Check for permission to read file
        sm.checkRead("/temp/tempFile");                         // UndCC_Valid
      }
      // Access the file
    } catch (SecurityException se) {
      // Log exception
    }
  }

    
  private void readSensitiveFile03() {
    try {
      SecurityManager sm = System.getSecurityManager();
      if (sm != null) {  // Check for permission to read file
        sm.checkRead("/temp/tempFile");                         // UndCC_Valid
      }
      // Access the file
    } catch (SecurityException se) {
      // Log exception
    }
  }

  public static void main(String[] args)
  {
    System.out.println("Hello test world!");	
  }
}


final class etest
{
  public void readSensitiveFile01() {
    try {
      SecurityManager sm = System.getSecurityManager();
      if (sm != null) {  // Check for permission to read file
        sm.checkLink("/temp/tempFile");                         // UndCC_Valid - MET03-J-EX0: Classes that are declared final are exempt from this rule because their member methods cannot be overridden.
      }
      // Access the file
    } catch (SecurityException se) {
      // Log exception
    }
  }
  
  
}
