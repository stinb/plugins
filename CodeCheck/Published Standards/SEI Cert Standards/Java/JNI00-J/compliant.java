
public final class NativeMethodWrapper {

  // Private native method
  private native void nativeOperation(byte[] data, int offset, int len);

  // Wrapper method performs SecurityManager and input validation checks
  public void doOperation(byte[] data, int offset, int len) {
    // Permission needed to invoke native method
    securityManagerCheck();
    
    if (data == null) {
      throw new NullPointerException();
    }
    
    // Copy mutable input
    data = data.clone();
    
    // Validate input
    if ((offset < 0) || (len < 0) || (offset > (data.length - len))) {
      throw new IllegalArgumentException();
    }
    
    nativeOperation(data, offset, len);  // UndCC_Valid
  }

  static {
    // Load native library in static initializer of class
    System.loadLibrary("NativeMethodLib");
  }
}
