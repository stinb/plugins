

public final class NativeMethod {

  // Public native method
  public native void nativeOperation(byte[] data, int offset, int len);

  // Wrapper method that lacks security checks and input validation
  public void doOperation(byte[] data, int offset, int len) {
    nativeOperation(data, offset, len);  // UndCC_Violation - native method wrapped in insecure wrapper
  }

  static {
    // Load native library in static initializer of class
    System.loadLibrary("NativeMethodLib");
  }
}
