import java.util.HashMap;
import java.util.Map;

public final class CreditCardNoncompliant { // UndCC_Violation
    private final int number;

    public CreditCardNoncompliant(int number) {
        this.number = number;
    }

    public boolean equals(Object o) {
        if (o == this) {
            return true;
        }
        if (!(o instanceof CreditCardNoncompliant)) {
            return false;
        }
        CreditCardNoncompliant cc = (CreditCardNoncompliant) o;
        return cc.number == number;
    }

    public static void main(String[] args) {
        Map<CreditCardNoncompliant, String> m = new HashMap<CreditCardNoncompliant, String>();
        m.put(new CreditCardNoncompliant(100), "4111111111111111");
        System.out.println(m.get(new CreditCardNoncompliant(100)));
    }
}
