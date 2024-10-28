
public class DoWhileExample {
  public static void main(String[] args) {
    int i=1;
    do {  
      System.out.println(i);  
      i++;  
    } while(i<=10);  /* UndCC_Valid */
  }
}


public class DoWhileExample2 {
  
  public static void main(String[] args) {
    
    int y=1;
    do {  
      System.out.println(y);  
      y++;  
    } 
    while(y<=10);  /* UndCC_Violation - while keyword not on same line as closing brace */
  }
}
