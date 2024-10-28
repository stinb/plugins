import java.util.Scanner;
import java.lang.*;

public class Main
{
    public static void main(String[] args) {
        int i = 15;

        String result = i % 2 == 0 ? "a" : i % 3 == 0 ? "b" : i % 5 == 0 ? "c" : i % 7 == 0 ? "d" : "e"; /* UndCC_Violation - nested ternary operators */

        System.out.print(result);
    }
}
