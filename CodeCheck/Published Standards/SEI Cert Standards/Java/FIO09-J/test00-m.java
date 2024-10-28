public class ConsoleWrite {
  public static void main(String[] args) {
    // Any input value > 255 will result in unexpected output
    
    int x = 1000;
    final int xf = 10;
    final int nxf = -1;
    final int xff;
    xff = TC.TX;
    
    final int xff1;
    xff1 = TC.x;
    
    final int xff2;
    xff2 = -1;
    
    
    System.out.write(10);       // UNDCC_Valid
    System.out.write(400);      // UNDCC_Violation
    System.out.write(x);        // UNDCC_FalseNeg - cant check non-final values
    x = 10;
    System.out.write(x);        // UNDCC_FalseNeg - cant check non-final values
    System.out.write(xf);       // UNDCC_Valid
    System.out.write(nxf);      // UNDCC_Violation
    System.out.write(xff);      // UNDCC_Violation
    System.out.write(xff1);     // UNDCC_FalseNeg - cant check non-final values
    System.out.write(xff2);     // UNDCC_Violation 
    
    System.out.flush();
    
    test(500, 500);             // UNDCC_Violation
    TC.t(300);                  // UNDCC_Violation
    
  }


  public static void test(int x, final int xf)
  {
    System.out.write(x);        // UNDCC_FalseNeg - cant check non-final values
    System.out.write(xf);       // Violation at call site
    
    System.out.flush();
  }
}

class TC
{
  final static int TX = 2000;
  static int x = -1;
  
  static void t(final int fx)
  {
     final int ttt;
     ttt = fx;
     System.out.write(ttt);      // Violation at call site
     
     final int ttx = fx;
     System.out.write(ttx);      // Violation at call site
  }
  
}
