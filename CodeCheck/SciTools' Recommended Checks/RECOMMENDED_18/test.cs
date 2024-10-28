class Program
{
  static void MyMethod()
  {
    int myNum = 15; // UndCC_Violation
    int myNum1 = 15; // UndCC_Valid
    Console.WriteLine(myNum1);
  }
}
