
public class Test
{ 
  public static void main(String[] args)
  {}
  
  public static final int a = 0; // UNDCC_Valid
  public static int a1; // UNDCC_Violation
  
  public static final String b = "b"; // UNDCC_Valid
  public static String b1; // UNDCC_Violation
}


