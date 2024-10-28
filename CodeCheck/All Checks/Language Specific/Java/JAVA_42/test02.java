class Test02 {
  public static void main(String[] args) {
    {

     }            // UNDCC_Violation

     if (true) {
/*comment*/     } // UNDCC_Violation

    int x;
    switch (x) {
        case 0:
            break;
        case 1:
            break;
        }         // UNDCC_Violation
  }
}
