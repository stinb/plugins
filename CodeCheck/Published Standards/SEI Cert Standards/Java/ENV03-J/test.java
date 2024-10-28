import java.net.*;
import java.security.*;
import java.lang.reflect.ReflectPermission;

final class ModURLClassLoader2 extends URLClassLoader {
  ModURLClassLoader2(final URL[] urls) {
    super(urls);
  }

  ModURLClassLoader2(final URL[] urls, final ClassLoader parent) {
    super(urls, parent);
  }   

  ModURLClassLoader2(final URL[] urls, final ClassLoader parent, final URLStreamHandlerFactory factory) {
    super(urls, parent, factory);
  }

  @Override
  protected PermissionCollection getPermissions(final CodeSource codesource) {
    PermissionCollection permissionCollection = super.getPermissions(codesource);
    permissionCollection.add(new ReflectPermission("suppressAccessChecks"));   // UndCC_Violation - Permission to create a class loader
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
