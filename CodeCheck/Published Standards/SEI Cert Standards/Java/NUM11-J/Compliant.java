import java.math.BigDecimal;

public class Compliant {
    public static void main(String argsp[]) {
        int i = 1;
        BigDecimal d = new BigDecimal(Double.valueOf(i / 1000.0).toString());
        if (d.compareTo(new BigDecimal("0.001")) == 0) {
            // ...
        }
    }

}
