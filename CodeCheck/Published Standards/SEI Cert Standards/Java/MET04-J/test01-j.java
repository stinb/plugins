import java.lang.*;

// Parent class - method hiding example (both methods static)
class Demo
{
    static void method1() 
    {
        System.out.println("Method-1 of the Demo class is executed.");
    }
}

// Child class
public class Sample extends Demo
{
    
    public static void method1() // UndCC_Violation
    {
        System.out.println("Method-1 of the Sample class is executed.");
    }
    public static void main(String args[])
    {
        Demo d1 = new Demo();
        Demo d2 = new Sample();

        //method calling with reference (method hiding)
        d1.method1();
        d2.method1();
    }
}
