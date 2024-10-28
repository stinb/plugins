import java.util.Date;

public class Test00 { // UndCC_Valid - class is immutable
  public static void main(String[] args) throws CloneNotSupportedException {
    // Class0: Cannot copy

    Class1 obj1a = new Class1(new Date());
    Class1 obj1b = new Class1(obj1a);
    System.out.println(obj1a != obj1b ? "Different objects" : "Same object");
    System.out.println(obj1a.getDate());
    System.out.println(obj1b.getDate());

    // Class2: Cannot instantiate

    Class3 obj3a = new Class3(new Date());
    Class3 obj3b = (Class3) obj3a.clone();
    System.out.println();
    System.out.println(obj3a != obj3b ? "Different objects" : "Same object");
    System.out.println(obj3a.getDate());
    System.out.println(obj3b.getDate());

    Class4 obj4a = new Class4(new Date());
    Class4 obj4b = (Class4) obj4a.clone();
    System.out.println();
    System.out.println(obj4a != obj4b ? "Different objects" : "Same object");
    System.out.println(obj4a.getDate());
    System.out.println(obj4b.getDate());

    Class5 obj5a = new Class5(new Date());
    Class5 obj5b = new Class5(obj5a.getDate());
    System.out.println();
    System.out.println(obj5a != obj5b ? "Different objects" : "Same object");
    System.out.println(obj5a.getDate());
    System.out.println(obj5b.getDate());
  }
}

public final class Class0 { // UndCC_Violation - clone method is not implemented
  private Date date;

  public Class0(Date d) {
    this.date = d;
  }

  public void setDate(Date d) {
    this.date = d;
  }

  public Date getDate() {
    return date;
  }
}

public final class Class1 { // UndCC_Valid - class is immutable
  private final Date date;

  public Class1(Class1 mc)  { // copy constructor
    this.date = new Date(mc.date.getTime());
  }

  public Class1(Date d) {
    this.date = new Date(d.getTime());
  }

  public Date getDate() {
    return (Date) date.clone();
  }
}

class Class2 { // UndCC_Valid - class is immutable
  private final Date date;

  private Class2(Date d) {
    this.date = new Date(d.getTime());
  }

  public Date getDate() {
    return (Date) date.clone();
  }

  public static Class2 getInstance(Class2 mc)  { // public static factory method
    return new Class2(mc.getDate());
  }
}

public final class Class3 implements Cloneable { // UndCC_Valid - clone is properly implemented
  private Date date;

  public Class3(Date d) {
    this.date = new Date(d.getTime());
  }

  public Date getDate() {
    return (Date) date.clone();
  }

  public void setDate(Date d) {
    this.date = (Date) d.clone();
  }

  public Object clone() throws CloneNotSupportedException { // clone method
    final Class3 cloned = (Class3) super.clone();
    cloned.date = (Date) date.clone();
    return cloned;
  }
}

public final class Class4 implements Cloneable { // UndCC_Valid - class is immutable
  private final Date date;

  public Class4(Date d) {
    this.date = new Date(d.getTime());
  }

  public Date getDate() {
    return (Date) date.clone();
  }

  public Object clone() { // clone method with final members
    Date d = (Date) date.clone();
    Class4 cloned = new Class4(d);
    return cloned;
  }
}

class UnmodifiableDateView extends Date { // UndCC_Violation - clone is not implemented
  private Date date;

  public UnmodifiableDateView(Date date) {
    this.date = date;
  }

  public void setTime(long date) { // only one mutator method is overridden - the class is mutable
    throw new UnsupportedOperationException();
  }
}

public final class Class5 { // UndCC_Valid - wrapper is implemented
  private Date date;

  public Class5(Date d) {
    this.date = d;
  }

  public void setDate(Date d) {
    this.date = (Date) d.clone();
  }

  public UnmodifiableDateView getDate() { // unmodifiable date wrapper
    return new UnmodifiableDateView(date);
  }
}
