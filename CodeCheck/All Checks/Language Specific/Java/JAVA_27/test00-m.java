public class Test {

int testingC = 100; /* mul
int testingC = 100;
*/ int testingCb = 100;
public static void main(String[] args){

  System.out.println("Hello World");
  
  if (testingC < testingC
    + testingC > 100
    && testingC >
      testingC)
      return; // UNDCC_Valid
    
  if (testingC < testingC
      + testingC > 100
      && testingC >
      testingC) // UNDCC_Violation
    return; 

  }
  
  
  private int x 
    = 5; // UNDCC_Valid
  
  private String y = "hello world";
  
  @Deprecated
  void funA(){
    
    if (x > 100 || 100 < x) {
      System.out.println("Invalid base!");
      continue;
    }
    
    x 
    = 5; // UNDCC_Violation
    
    if(x /*yo yo yo*/ == 5)
      x = 10;
    
    if(x == 10)
    x = 15;
    
    y = "yo yo"
    ; // UNDCC_Violation

    int
      x
        =
          10
            ; // UNDCC_Valid
  
    if(x == 5) x = 10; private int zzz =
    10; // UNDCC_Violation
  
            private boolean xb  =
              False;
          
          private
          boolean xba = False; // UNDCC_Violation
          
          int
            x
              =
              10 // UNDCC_Violation
              ;  // UNDCC_Violation
          
    if(x == 5) { x = 4; }
    else x = 10;
    
    switch (x) {
      case 1:  x = 100;
        break;
      case 2:  
        x = 200; // UNDCC_Valid
      case 3: {break;}
      case 4:
      x = 5; // Violation if checking switches
  
    }
    
    int i=1;    
    do{    
      System.out.println(i);    
      i++;    
    }while(i<=10);    
    
    
  }
  
  void funB(
    int a,
    int b,
    int c) // UNDCC_Valid
  {
  
  
  } 
  
  void funC(
    int a,
    int b,
    int c
  ) // UNDCC_Violation
  {}
  
  void funD(
  int a, // UNDCC_Violation
   int c)
  {}
  
  void funE() {
    int x,y,z = 0;
    if ( x ==
      z
      && y ==
      z && x == 0) // UNDCC_Violation 
    return;
  }

}
