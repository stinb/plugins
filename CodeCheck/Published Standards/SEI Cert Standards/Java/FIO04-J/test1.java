import java.sql.*;
import java.io.*;

class Test1 {

  // Noncompliant Code Example (No Variable Assignment)
  public int processFile0(String fileName)
                        throws IOException, FileNotFoundException {
    new FileInputStream(fileName); // UndCC_Violation
    return 1;
  }

  // Compliant Solution (No Variable Assignment)
  public int processFile1(String fileName)
                        throws IOException, FileNotFoundException {
    try (new FileInputStream(fileName)) { // UndCC_Valid
      System.out.println("Reading file " + fileName);
    }
    return 1;
  }

  // Noncompliant Code Example (No Variable Assignment)
  public void getResults0(String sqlQuery) {
    try {
      getConnection(); // UndCC_Violation
    } catch (SQLException e) { /* Forward to handler */ }
  }

}
