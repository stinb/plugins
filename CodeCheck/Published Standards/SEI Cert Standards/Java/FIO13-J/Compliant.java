import java.util.logging.*;
import java.net.InetAddress;
import java.net.UnknownHostException;
// import java.lang.Exception;

public class Compliant {
    public void logRemoteIPAddress(String name) {
        Logger logger = Logger.getLogger("com.organization.Log");
        InetAddress machine = null;
        try {
            machine = InetAddress.getByName(name);
        } catch (UnknownHostException e) {
            // Exception e = MyExceptionReporter.handle(e);
        } catch (SecurityException e) {
            // Exception e = MyExceptionReporter.handle(e);
            logger.finest(name + "," + machine.getHostAddress() + "," +
                    e.toString()); // UndCC_Valid
        }
    }

}
