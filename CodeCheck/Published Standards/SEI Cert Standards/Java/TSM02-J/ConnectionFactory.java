import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public final class ConnectionFactory {
    private static Connection dbConnection;
    // Other fields ...

    static {
        // Initialize a database connection
        try {
            dbConnection = DriverManager.getConnection("connection string");
        } catch (SQLException e) {
            dbConnection = null;
        }
        // Other initialization (do not start any threads)
    }

    // ...
}