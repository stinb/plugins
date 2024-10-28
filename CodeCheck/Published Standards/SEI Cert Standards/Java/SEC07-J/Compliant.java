import java.net.URLClassLoader;
import java.security.PermissionCollection;
import java.net.URL;
import java.security.CodeSource;

public class Compliant extends URLClassLoader {
    Compliant(URL[] urls) {
        super(urls);
    }

    protected PermissionCollection getPermissions(CodeSource cs) {
        PermissionCollection pc = super.getPermissions(cs);
        // Allow exit from the VM anytime
        pc.add(new RuntimePermission("exitVM"));
        return pc;
    }

}
