import java.net.*;
import java.security.*;
import java.lang.reflect.ReflectPermission;

final class ModURLClassLoader extends URLClassLoader {
  ModURLClassLoader(final URL[] urls) {
    super(urls);
  }

  ModURLClassLoader(final URL[] urls, final ClassLoader parent) {
    super(urls, parent);
  }   

  ModURLClassLoader(final URL[] urls, final ClassLoader parent, final URLStreamHandlerFactory factory) {
    super(urls, parent, factory);
  }

  @Override
  protected PermissionCollection getPermissions(final CodeSource codesource) {
    String my_permissions = "suppressAccessChecks";
    ReflectPermission rp = new ReflectPermission(my_permissions);
    PermissionCollection permissionCollection = super.getPermissions(codesource);
    permissionCollection.add(rp);   // UndCC_Violation - Permission to create a class loader
    return permissionCollection;
  }
  
  @Override
  protected Class<?> loadClass(final String name, final boolean resolve) throws ClassNotFoundException {
    SecurityManager sm = System.getSecurityManager();
    if (sm != null) {
      int i = name.lastIndexOf('.');
      if (i != -1) {
        sm.checkPackageAccess(name.substring(0, i));
      }
    }
    return super.loadClass(name, resolve);
  }
}
