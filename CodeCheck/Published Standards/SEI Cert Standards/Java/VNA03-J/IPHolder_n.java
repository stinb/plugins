import java.util.List;
import java.net.InetAddress;
import java.util.Collections;
import java.util.ArrayList;

final class IPHolder_n {
    private final List<InetAddress> ips = Collections.synchronizedList(new ArrayList<InetAddress>());

    public void addAndPrintIPAddresses(InetAddress address) { 
        ips.add(address); // UndCC_Violation
        InetAddress[] addressCopy = (InetAddress[]) ips.toArray(new InetAddress[0]);
        // Iterate through array addressCopy ...
    }
}
