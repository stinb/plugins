public class test00 {
  
  public static void NaNComparison() {
    double x = 0.0;
    double result = Math.cos(1/x); // Returns NaN if input is infinity
    if (result == Double.NaN) { // UNDCC_Violation - Comparison is always false!
      System.out.println("result is NaN");
    }
  }
  
  
  public static void NaNComparisonCompliant() {
    double x = 0.0;  
    double result = Math.cos(1/x); // Returns NaN when input is infinity
    if (Double.isNaN(result)) { // UNDCC_Valid
      System.out.println("result is NaN");
    }
  }
  
  
  public static void f01() {
    
    double d = Double.NaN; 
    boolean f = d == 0.0; // Potential variable check must be turned on for violation
    d = 100;
    
    final double c;
    c = Double.NaN;
    f = c == 0.0; // Potential variable check must be turned on for violation
    
    final double fd = Double.NaN;
    
    if(10 != fd) // UNDCC_Violation
    {}
    
    while((Float.NaN) // UNDCC_Violation
          > f02(0f)) {}
    
    f03( 0, Float.NaN); // UNDCC_Violation
    f03( f02(0f), fd); // UNDCC_Violation
  }
  
  public static int f02( Float i){ 
    double falseneg = Double.NaN;
    falseneg = 0.0;
    
    if(falseneg == 0.0) // Potential variable check must be turned on for violation
    {}
    
    i = Float.NaN;
    
    if (i > 0) // Potential variable check must be turned on for violation
    {}
    return Math.round(i);
  }
  
  
  public static int f03(int x, final double i) {
    
    boolean g = i <= 0.0;

    if(i != 5 && g) 
    {
      return 9;
    }
    return 0;
  }
  
  
  public static void main(String[] args) {
    NaNComparison();
    NaNComparisonCompliant();
    f01();
    f02(Float.NaN); // Potential variable check must be turned on for violation
  }
  
}



