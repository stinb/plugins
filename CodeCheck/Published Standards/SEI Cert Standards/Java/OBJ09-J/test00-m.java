class Test
{ 
  public static void main(String[] args)
  {
    TClass auth = new TClass();
    TClass x = new TClass();
    TClass y = new TClass();
    
    // Determine whether object auth has required/expected class object
    if (auth.getClass().getName().equals("TClass")) { System.out.println("Match"); } // UNDCC_Violation
    
    if (auth.getClass() == TClass.class) { System.out.println("Match"); } // UNDCC_Valid
    
    // Determine whether objects x and y have the same class name
    if (x.getClass().getName().equals(y.getClass().getName())) { System.out.println("Match"); } // UNDCC_Violation
    
    if (x.getClass().getName() == y.getClass().getName()) { System.out.println("Match"); } // UNDCC_Violation
    
    // Determine whether objects x and y have the same class
    if (x.getClass() == y.getClass()) { System.out.println("Match"); } // UNDCC_Valid
  }
  void test(){}
}

class SecondTest
{
  String t()
  {
    TClass x = new TClass();
    return x.getClass().getName();
  }
  
}

class TClass{}
