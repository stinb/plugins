
class Test3 {
public static void workWith(int i) {
  byte b = (byte) i;  // UNDCC_Violation
  // Work with b
}
}
