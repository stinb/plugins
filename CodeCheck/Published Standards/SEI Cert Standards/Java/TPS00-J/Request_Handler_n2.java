import java.net.ServerSocket;
import java.io.IOException;

final class RequestHandler_n_2 {
    private final Helper helper = new Helper();
    private final ServerSocket server;

    private RequestHandler_n_2(int port) throws IOException {
        server = new ServerSocket(port);
    }

    public static RequestHandler_n_2 newInstance() throws IOException {
        return new RequestHandler_n_2(0); // Selects next available port
    }

    public void handleRequest() {
        Thread thread = new Thread(new Runnable() {
            public void run() {
                try {
                    helper.handle(server.accept());
                } catch (IOException e) {
                    // Forward to handler
                }
            }
        });

        thread.start(); // UndCC_Violation
    }

}
