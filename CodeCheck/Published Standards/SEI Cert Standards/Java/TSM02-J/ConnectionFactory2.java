import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public final class ConnectionFactory2 {
    private static final ThreadLocal<Connection> connectionHolder = new ThreadLocal<Connection>() {
        @Override
        public Connection initialValue() {
            try {
                Connection dbConnection = DriverManager.getConnection("connection string");
                return dbConnection;
            } catch (SQLException e) {
                return null;
            }
        }
    };

    // Other fields ...

    static {
        // Other initialization (do not start any threads)
    }

    public static Connection getConnection() {
        Connection connection = connectionHolder.get();
        if (connection == null) {
            throw new IllegalStateException("Error initializing connection");
        }
        return connection;
    }

    public static void main(String[] args) {
        // ...
        Connection connection = getConnection();
    }
}