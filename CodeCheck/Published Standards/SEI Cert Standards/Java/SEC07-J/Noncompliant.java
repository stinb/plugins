import java.net.URLClassLoader;
import java.security.PermissionCollection;
import java.net.URL;
import java.security.CodeSource;
import java.security.Permissions;

public class Noncompliant extends URLClassLoader {
    Noncompliant(URL[] urls) {
        super(urls);
    }

    protected PermissionCollection getPermissions(CodeSource cs) { // UndCC_Violation
        PermissionCollection pc = new Permissions();
        // Allow exit from the VM anytime
        pc.add(new RuntimePermission("exitVM"));
        return pc;
    }

}
