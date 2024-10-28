class Test1 {
  public static void workWith(int i) {
    // Check whether i is within byte range
    if ((i < Byte.MIN_VALUE) || (i > Byte.MAX_VALUE)) {
      throw new ArithmeticException("Value is out of range");
    }
 
    byte b = (byte) i; // UNDCC_Valid
    // Work with b
  }
}
