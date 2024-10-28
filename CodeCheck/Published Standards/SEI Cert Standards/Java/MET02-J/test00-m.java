
@Deprecated
public class Worker {
  public static void main(String[] args)
  {
    int x = calculate(); // UNDCC_CF1_Violation
    x = BIGX; // UNDCC_Violation
  }

  public static int calculate() {
    Worker nw = new Worker(); // UNDCC_Violation
    return 10;
  }

  @Deprecated
  public static int BIGX = 0;
  
  class SubWorker {
    
    class SubSubWorker {
      
      class SubSubSubWorker {
        
        int xx = calculate(); // UNDCC_CF1_Violation
        
      }
    }
    
  }
}

@Deprecated
abstract class testab {}

class test {
  Worker nw = new Worker(); // UNDCC_Violation
  int x = Worker.calculate(); // UNDCC_CF1_Violation
  void testm(){
    x = Worker.BIGX; // UNDCC_Violation
  }
  testab xab; // UNDCC_Violation
  
  @Deprecated
  void t() {
    tt();
  }
  
  @Deprecated
  void tt() {
    t();
  }
    
}
