import java.math.BigDecimal;

public class Noncompliant {
    public void foo() {
        // Prints 0.1000000000000000055511151231257827021181583404541015625
        // when run in FP-strict mode
        System.out.println(new BigDecimal(0.1)); // UndCC_Violation
    }
}
