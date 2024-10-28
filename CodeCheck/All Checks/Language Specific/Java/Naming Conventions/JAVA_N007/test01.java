class Main {

  public static void main(String[] args) {
    
    static final int bad_constant=1;      // UNDCC_Violation
    static final double PI=3.14;
    static final double GOOD_CONSTANT=2.0;
    
    System.out.println("Enter two numbers");
    int first = 10;
    int second = 20;
    
    System.out.println(first + " " + second);

    // add two numbers
    int sum = first + second;
    System.out.println("The sum is: " + sum);
  }
}
