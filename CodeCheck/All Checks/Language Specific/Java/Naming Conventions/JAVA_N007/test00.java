class Main {

  public static void main(String[] args) {
    
    static final int Another_Bad_Constant=1;      // UNDCC_Violation
    static final int AND_ANOTHER_bad_CONSTANT=10; // UNDCC_Violation
    static final int DEPENDS_123=123;             // UNDCC_Valid valid if numbers option enabled, otherwise invalid
    
    System.out.println("Enter two numbers");
    int first = 10;
    int second = 20;
    
    System.out.println(first + " " + second);

    // add two numbers
    int sum = first + second;
    System.out.println("The sum is: " + sum);
  }
}
