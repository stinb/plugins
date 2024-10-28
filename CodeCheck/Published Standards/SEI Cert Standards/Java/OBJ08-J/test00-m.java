class Coordinates 
{
  private int x;
  private int y;
  
  
  enum Level 
  {
    LOW,
    MEDIUM,
    HIGH,
  }


  class PointA 
  {
    public void getPointA() 
    {
      System.out.println("(" + x + "," + y + ")"); // UNDCC_Violation
    }
    
    private void getPointB()
    {
      System.out.println("(" + x + "," + y + ")"); // UNDCC_Valid
    }
  }
  
  private class PointB 
  {
    public void getPointA() 
    {
      System.out.println("(" + x + "," + y + ")"); // UNDCC_Valid
    }
    
    private void getPointB()
    {
      System.out.println("(" + x + "," + y + ")"); // UNDCC_Valid
    }
  }
}


class CordinatesB 
{
  
  private class PointB 
  {
    private void getPoint() 
    {
      System.out.println("12");
    }
    
    void hello() {}
    
  }
}

public class AnotherClass 
{
  public static void main(String[] args) 
  {
    Coordinates c = new Coordinates();
    Coordinates.PointA  p = c.new PointA ();
    p.getPointA();
  }
}

