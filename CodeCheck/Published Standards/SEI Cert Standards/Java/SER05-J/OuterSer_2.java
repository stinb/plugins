import java.io.Serializable;

public class OuterSer_2 implements Serializable {
    private int rank;

    static class InnerSer implements Serializable { // UndCC_Valid
        protected String name;
        // ...
    }
}
