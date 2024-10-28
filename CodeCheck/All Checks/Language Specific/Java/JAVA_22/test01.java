import java.lang.*;

public class Main
{
    public static int getAmount() {
        return 10;
    }

    public static void count() {
        int i = 0;
        for (i = 0; i < 5; i++) {
            System.out.print(i);
        }
        i = getAmount();  /* UndCC_Violation - variable is being used as a for loop incrementing variable, then later used to hold an amount */
    }

    public static void main(String[] args) {
        System.out.print("Hello");
        count();
    }
}
