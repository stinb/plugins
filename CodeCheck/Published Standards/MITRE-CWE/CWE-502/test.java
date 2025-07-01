import java.io.*;
import java.net.*;

public class UnsafeNetworkDeserialization {
  public static void main(String[] args) {
    try {
      // Start a server on port 8000
      ServerSocket serverSocket = new ServerSocket(8000);
      System.out.println("Server listening on port 8000...");

      while (true) {
        // Accept client connection
        Socket clientSocket = serverSocket.accept();
        System.out.println("Client connected: " + clientSocket.getInetAddress());

        // Deserialize network data using ObjectInputStream (UNSAFE)
        ObjectInputStream ois = new ObjectInputStream(clientSocket.getInputStream());
        Object data = ois.readObject(); // UndCC_Violation

        // Process the deserialized data
        System.out.println("Received data: " + data.toString());

        // Send response
        PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
        out.println("Processed data: " + data.toString());

        // Clean up
        ois.close();
        clientSocket.close();
      }
    } catch (Exception e) {
      System.err.println("Error: " + e.getMessage());
    }
  }
}

