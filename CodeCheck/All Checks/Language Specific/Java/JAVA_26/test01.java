public class Main
{
  public static void main(String[] args) 
  {
    /* UndCC_Valid */
    int size; 
    int capacity;

    size = 4; capacity = 10;  /* UndCC_Violation - multiple expression statements on same line */


    if (size < capacity)
    {
      System.out.println(size + " is within capacity boundary");
    }
  }
}
