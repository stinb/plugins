import java.util.Objects;

public final class SpaceShuttle {

  /**
  * The non-nullity of name is part of the contract of the
  * constructor.
  *
  * @param name is non-null.
  */
  public SpaceShuttle(String name) {/* UndCC_Violation */
    //WRONG way:
    assert name != null : "Name must not be null";  
    //RIGHT way:
    this.name = Objects.requireNonNull(name);
  }

  public String getName() {
    return name;
  }

  //..elided

  // PRIVATE
  private final String name;
}
