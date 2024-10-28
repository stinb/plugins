class Test {
    public static void main(String[] args) {
      float a = Float.MIN_VALUE;
      float b = Float.MAX_VALUE;
      short c = (short) a; // UNDCC_Violation
      short d = (short) b; // UNDCC_Violation

      float e = Float.MIN_VALUE;
      float f = Float.MAX_VALUE;
      if ((e < Short.MIN_VALUE) || (e > Short.MAX_VALUE) ||
          (f < Short.MIN_VALUE) || (f > Short.MAX_VALUE)) {
        throw new ArithmeticException ("Value is out of range");
      }
   
      short g = (short) e; // UNDCC_Valid
      short h = (short) f; // UNDCC_Valid
      // Other operations}
   
      double i = Double.MIN_VALUE;
      double j = Double.MAX_VALUE;
      float k = (float) i; // UNDCC_Violation
      float l = (float) j; // UNDCC_Violation

      double m = Double.MIN_VALUE;
      double n = Double.MAX_VALUE;
      if ((m < Float.MIN_VALUE) || (m > Float.MAX_VALUE) ||
          (n < Float.MIN_VALUE) || (n > Float.MAX_VALUE)) {
        throw new ArithmeticException ("Value is out of range");
      }
       
      float o = (float) m; // UNDCC_Valid
      float p = (float) n; // UNDCC_Valid
      // Other operations
  }
}
