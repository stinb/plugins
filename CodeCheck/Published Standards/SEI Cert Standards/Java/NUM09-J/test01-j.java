public class ExampleClass {
  public static void main(String[] args) {
    float i=1.0;
    do{
      System.out.println(i);
      i++;
    }while(i<=10);  /* UndCC_Violation float used in a do/while loop */

    double j=12.223;
    while(j <= 13.0){  /* UndCC_Violation double used in a while loop */
      System.out.println(j);
      j+=1.0;
    }
  }
}
