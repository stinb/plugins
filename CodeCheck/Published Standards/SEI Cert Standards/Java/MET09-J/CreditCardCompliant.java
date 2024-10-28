import java.util.Map;
import java.util.HashMap;

public final class CreditCardCompliant {
    private final int number;

    public CreditCardCompliant(int number) {
        this.number = number;
    }

    public boolean equals(Object o) {
        if (o == this) {
            return true;
        }
        if (!(o instanceof CreditCardCompliant)) {
            return false;
        }
        CreditCardCompliant cc = (CreditCardCompliant) o;
        return cc.number == number;
    }

    public int hashCode() {
        int result = 17;
        result = 31 * result + number;
        return result;
    }

    public static void main(String[] args) {
        Map<CreditCardCompliant, String> m = new HashMap<CreditCardCompliant, String>();
        m.put(new CreditCardCompliant(100), "4111111111111111");
        System.out.println(m.get(new CreditCardCompliant(100)));
    }
}
