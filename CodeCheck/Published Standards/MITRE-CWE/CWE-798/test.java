public class AdminVerifier {
int VerifyAdmin(String password) {
  if (!password.equals("Mew!")) { // UndCC_Violation
    return 0;
  }
  //Diagnostic Mode
  return 1;
}
}
