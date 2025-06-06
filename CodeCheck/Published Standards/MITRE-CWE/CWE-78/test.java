import java.util.Scanner;

class Main {
  public static void main(String[] args) {
    Scanner myObj = new Scanner(System.in);

    String command = myObj.nextLine();
    Runtime.getRuntime().exec(command); // UndCC_Violation
  }
}
