public class Main2
{
  public static void main2(String[] args) 
  {
    
    int size; int capacity;  /* UndCC_Violation - multiple declaration statements on same line */

    /* UndCC_Valid */
    size = 4; 
    capacity = 10;


    if (size < capacity)
    {
      System.out.println(size + " is within capacity boundary");; /* UndCC_Violation - double semicolon interpreted as an empty statement */
    }  
  }
}

