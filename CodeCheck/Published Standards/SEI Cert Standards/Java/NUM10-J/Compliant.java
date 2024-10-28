import java.math.BigDecimal;

public class Compliant {
    public void foo() {
        // Prints 0.1
        // when run in FP-strict mode
        System.out.println(new BigDecimal("0.1")); // UndCC_Valid
    }
}
