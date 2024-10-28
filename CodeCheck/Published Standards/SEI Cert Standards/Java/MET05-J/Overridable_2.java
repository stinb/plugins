class Canine {
  public Canine() {
    Bark();  // UNDCC_Violation overridable method called in a constructor
  }

  public void Bark() {
    System.out.println("Woof!");
  }
}

class Bulldog extends Canine {
  private String breed = "bulldog";

  // overrides 'bark' method
  public void Bark() {
    System.out.println("Woof! I am a " + breed);
    // ...
  }
}

public class Overridable {
  public static void main(String[] args) {
    Canine bc = new Canine();
    Canine sc = new Bulldog();
  }
}
