import java.util.Map;
import java.util.HashMap;

public final class IdNumberCompliant {
    private final int number;

    public IdNumberCompliant(int number) {
        this.number = number;
    }

    public boolean equals(Object o) {
        if (o == this) {
            return true;
        }
        if (!(o instanceof IdNumberCompliant)) {
            return false;
        }
        IdNumberCompliant id = (IdNumberCompliant) o;
        return id.number == number;
    }

    public int hashCode() {
        int result = 17;
        result = 31 * result + number;
        return result;
    }

    public static void main(String[] args) {
        Map<IdNumberCompliant, String> m = new HashMap<IdNumberCompliant, String>();
        m.put(new IdNumberCompliant(100), "4111111111111111");
        System.out.println(m.get(new IdNumberCompliant(100)));
    }
}
