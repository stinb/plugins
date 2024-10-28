import java.io.Serializable;

public class OuterSer implements Serializable {
  private int rank;

  class InnerSer { // UndCC_Valid
    protected String name;
    // ...
  }
}
