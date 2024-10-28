import java.util.HashMap;
import java.util.Map;

public final class IdNumberNoncompliant { // UndCC_Violation
    private final int number;

    public IdNumberNoncompliant(int number) {
        this.number = number;
    }

    public boolean equals(Object o) {
        if (o == this) {
            return true;
        }
        if (!(o instanceof IdNumberNoncompliant)) {
            return false;
        }
        IdNumberNoncompliant id = (IdNumberNoncompliant) o;
        return id.number == number;
    }

    public static void main(String[] args) {
        Map<IdNumberNoncompliant, String> m = new HashMap<IdNumberNoncompliant, String>();
        m.put(new IdNumberNoncompliant(100), "4111111111111111");
        System.out.println(m.get(new IdNumberNoncompliant(100)));
    }
}
