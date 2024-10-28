class Test2 {
   public static void workWith(int i) {
     byte b = (byte)(i % 0x100); // UNDCC_FalsePos - can't identify type of resulting arithmetic
     // Work with b
   }
}
