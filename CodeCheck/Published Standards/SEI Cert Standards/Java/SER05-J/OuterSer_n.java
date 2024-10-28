import java.io.Serializable;

public class OuterSer_n implements Serializable {
    private int rank;

    class InnerSer implements Serializable { // UndCC_Violation
        protected String name;
        // ...
    }
}