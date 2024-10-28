import java.net.*;
import java.security.*;
import java.lang.reflect.ReflectPermission;

final class ModURLClassLoader3 extends URLClassLoader {
  ModURLClassLoader2(final URL[] urls) {
    super(urls);
  }

  ModURLClassLoader3(final URL[] urls, final ClassLoader parent) {
    super(urls, parent);
  }   

  ModURLClassLoader3(final URL[] urls, final ClassLoader parent, final URLStreamHandlerFactory factory) {
    super(urls, parent, factory);
  }

 protected PermissionCollection getPermissions(CodeSource cs) {
  PermissionCollection pc = super.getPermissions(cs); // UndCC_Valid
  // Other permissions
  return pc;
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
