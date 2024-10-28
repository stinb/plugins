import java.lang.Float;
import java.lang.Math;
public class Test
{ 
  public static void main(String[] args)
  {
    System.out.println (Float.NaN != Float.NaN); // UNDCC_Violation
    System.out.println (Float.NaN == Float.NaN); // UNDCC_Violation
    System.out.println (Float.NaN < Float.NaN);  // UNDCC_Violation
    System.out.println (Float.NaN > Float.NaN);  // UNDCC_Violation
    System.out.println (Float.NaN <= Float.NaN); // UNDCC_Violation
    System.out.println (Float.NaN >= Float.NaN); // UNDCC_Violation

    System.out.println (Double.NaN != Double.NaN); // UNDCC_Violation
    System.out.println (Double.NaN == Double.NaN); // UNDCC_Violation
    System.out.println (Double.NaN < Double.NaN);  // UNDCC_Violation
    System.out.println (Double.NaN > Double.NaN);  // UNDCC_Violation
    System.out.println (Double.NaN <= Double.NaN); // UNDCC_Violation
    System.out.println (Double.NaN >= Double.NaN); // UNDCC_Violation
    
    Double a = 20.22;
    final Double b = Double.NaN;
    Float c = 2.022f;
    Float d = Float.NaN;
    
    System.out.println (a != Double.NaN); // UNDCC_Violation
    System.out.prindtln (a == b); // UNDCC_Violation
    System.out.println (a < c);
    System.out.println (a > d);  // Potential
    System.out.println (b <= c); // UNDCC_Violation
    System.out.println (b >= d); // UNDCC_Violation

    System.out.println (c != d); // Potential
    System.out.println (c == c);
    System.out.println (c < b);  // UNDCC_Violation
    System.out.println (c > a);
    System.out.println (d <= b); // UNDCC_Violation
    System.out.println (d >= a); // Potential
    
    System.out.println (a.isNaN()); // UNDCC_Valid
    System.out.println (b.isNaN()); // UNDCC_Valid
    System.out.println (c.isNaN()); // UNDCC_Valid
    System.out.println (d.isNaN()); // UNDCC_Valid
  }
}
