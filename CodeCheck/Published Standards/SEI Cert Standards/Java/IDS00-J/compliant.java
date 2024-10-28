
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

class Login {
  public Connection getConnection() throws SQLException {
    DriverManager.registerDriver(new
                                 com.microsoft.sqlserver.jdbc.SQLServerDriver());
    String dbConnection =
      PropertyManager.getProperty("db.connection");
    // Can hold some value like
    // "jdbc:microsoft:sqlserver://<HOST>:1433,<UID>,<PWD>"
    return DriverManager.getConnection(dbConnection);
  }

  String hashPassword(char[] password) {
    // Create hash of password
  }

  public void doPrivilegedAction(
  String username, char[] password
  ) throws SQLException {
    Connection connection = getConnection();
    if (connection == null) {
      // Handle error
    }
    try {
      String pwd = hashPassword(password);

      // UndCC_Validate username length
      if (username.length() > 8) {
        // Handle error
      }

      String sqlString =
        "select * from db_user where username=? and password=?";
      PreparedStatement stmt = connection.prepareStatement(sqlString);
      stmt.setString(1, username);
      stmt.setString(2, pwd);
      ResultSet rs = stmt.executeQuery();  /* UndCC_Valid */
      if (!rs.next()) {
        throw new SecurityException("User name or password incorrect");
      }

      // Authenticated; proceed
    } finally {
      try {
        connection.close();
      } catch (SQLException x) {
        // Forward to handler
      }
    }
  }
}
