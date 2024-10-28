import java.sql.*;
import java.io.*;

class Test0 {

  // Noncompliant Code Example (File Handle)
  public int processFile0(String fileName)
                        throws IOException, FileNotFoundException {
    FileInputStream stream = new FileInputStream(fileName); // UndCC_Violation
    BufferedReader bufRead =
        new BufferedReader(new InputStreamReader(stream));
    String line;
    while ((line = bufRead.readLine()) != null) {
      sendLine(line);
    }
    return 1;
  }


  // Compliant Solution
  public int processFile1(String fileName)
                        throws IOException, FileNotFoundException {
    try {
      final FileInputStream stream = new FileInputStream(fileName); // UndCC_Valid
      try {
        final BufferedReader bufRead =
            new BufferedReader(new InputStreamReader(stream));

        String line;
        while ((line = bufRead.readLine()) != null) {
          sendLine(line);
        }
      } finally {
        if (stream != null) {
          try {
            stream.close();
          } catch (IOException e) {
            // Forward to handler
          }
        }
      }
    } catch (IOException e) {
      // Forward to handler
    }
  }


  // Compliant Solution (try-with-resources)
  public int processFile2(String fileName)
                        throws IOException, FileNotFoundException {
    try (FileInputStream stream = new FileInputStream(fileName); // UndCC_Valid
        BufferedReader bufRead =
            new BufferedReader(new InputStreamReader(stream))) {

      String line;
      while ((line = bufRead.readLine()) != null) {
        sendLine(line);
      }
    } catch (IOException e) {
      // Forward to handler
    }
  }


  // Noncompliant Code Example (SQL Connection)
  public void getResults0(String sqlQuery) {
    try {
      Connection conn = getConnection(); // UndCC_Violation
      Statement stmt = conn.createStatement();
      ResultSet rs = stmt.executeQuery(sqlQuery);
      processResults(rs);
      stmt.close(); conn.close();
    } catch (SQLException e) { /* Forward to handler */ }
  }


  // Noncompliant Code Example
  public void getResults1(String sqlQuery) {
    Statement stmt = null;
    ResultSet rs = null;
    Connection conn = getConnection(); // UndCC_Violation
    try {
      stmt = conn.createStatement();
      rs = stmt.executeQuery(sqlQuery);
      processResults(rs);
    } catch(SQLException e) {
      // Forward to handler
    } finally {
      rs.close();
      stmt.close(); conn.close();
    }
  }

  // Noncompliant Code Example
  public void getResults2(String sqlQuery) {
    Statement stmt = null;
    ResultSet rs = null;
    Connection conn = getConnection(); // UndCC_Violation
    try {
      stmt = conn.createStatement();
      rs = stmt.executeQuery(sqlQuery);
      processResults(rs);
    } catch (SQLException e) {
      // Forward to handler
    } finally {
      if (rs != null) {
        rs.close();
      }
      if (stmt != null) {
        stmt.close();
      } if (conn !=null) {
          conn.close();
        }
    }
  }

  // Compliant Solution
  public void getResults2(String sqlQuery) {
    Statement stmt = null;
    ResultSet rs = null;
    Connection conn = getConnection(); // UndCC_Valid
    try {
      stmt = conn.createStatement();
      rs = stmt.executeQuery(sqlQuery);
      processResults(rs);
    } catch (SQLException e) {
      // Forward to handler
    } finally {
      try {
        if (rs != null) {rs.close();}
      } catch (SQLException e) {
        // Forward to handler
      } finally {
        try {
          if (stmt != null) {stmt.close();}
        } catch (SQLException e) {
          // Forward to handler
        } finally {
          try {
            if (conn != null) {conn.close();}
          } catch (SQLException e) {
            // Forward to handler
          }
        }
      }
    }
  }

  // Compliant Solution (try-with-resources)
  public void getResults2(String sqlQuery) {
    try (Connection conn = getConnection(); // UndCC_Valid
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery(sqlQuery)) {
      processResults(rs);
    } catch (SQLException e) {
      // Forward to handler
    }
  }

}
