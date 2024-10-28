import java.net.ServerSocket;
import java.io.IOException;

final class RequestHandler_n_n {
    private final Helper helper = new Helper();
    private final ServerSocket server;

    private RequestHandler_n_n(int port) throws IOException {
        server = new ServerSocket(port);
    }

    public static RequestHandler_n_n newInstance() throws IOException {
        return new RequestHandler_n_n(0); // Selects next available port
    }

    public void handleRequest() {
        new Thread(new Runnable() {
            public void run() {
                try {
                    helper.handle(server.accept());
                } catch (IOException e) {
                    // Forward to handler
                }
            }
        }).start();     //UndCC_Violation
    }

}
