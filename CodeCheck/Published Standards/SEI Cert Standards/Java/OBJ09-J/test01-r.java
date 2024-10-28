class Test01
{ 
  public static void main(String[] args) {
    TClass x = new TClass();
    TClass y = new TClass();

    final String xClass = x.getClass().getName();
    String yClass = x.getClass().getName();
    
    final String tClass;
    tClass = x.getClass().getName();
    
    String aClass = "hello";
    

    if (xClass.contentEquals("TClass")) { // UNDCC_Violation
      System.out.println("Match");
    }
    
    if (yClass.equals("TClass")) { // potential, could lead to false positives
      System.out.println("Match");
    }
    
    if( xClass != yClass){ // UNDCC_Violation
      System.out.println("Not Match");
    }
    
    if (x.getClass().getName().contentEquals("TClass")) { // UNDCC_Violation
      System.out.println("Match");
    }
    
    if (tClass.equals("TClass")) { // UNDCC_Violation
      System.out.println("Match");
    }
    
    if (aClass.equals("TClass")) { // potential - Since non-final variables can't be tracked 
      System.out.println("Match");
    }
    
    aClass = x.getClass().getName();
    
    if (aClass.equals("TClass")) { // potential, could lead to false positives
      System.out.println("Match");
    }
    
    tcCompare(x.getClass().getName()); // UNDCC_Violation
    tcCompare(xClass); // UNDCC_Violation
    
    tcComparenf(x.getClass().getName()); // potential, could lead to false positives
    tcComparenf(yClass); // potential, could lead to false positives
    
    ntCompare(xClass);
  }
  
  public static boolean tcCompare(final String x){
    return (x == "TClass");
  }
  
  public static boolean tcComparenf(String x){
    return (x == "TClass");
  }
  
  public static String ntCompare(final String x){
    return x +  "good";
  }
}

