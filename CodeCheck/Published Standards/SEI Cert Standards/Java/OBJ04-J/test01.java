import java.util.Date;

public class Test01 { // UndCC_Valid - class is immutable
  public static void main(String[] args) throws CloneNotSupportedException {
    Class6 obj6a = new Class6(new Date());
    Class6 obj6b = (Class6) obj6a.clone();
    System.out.println();
    System.out.println(obj6a != obj6b ? "Different objects" : "Same object");
    System.out.println(obj6a.getDate());
    System.out.println(obj6b.getDate());

    Class7 obj7a = new Class7(new Date());
    Class7 obj7b = (Class7) obj7a.clone();
    System.out.println();
    System.out.println(obj7a != obj7b ? "Different objects" : "Same object");
    System.out.println(obj7a.getDate());
    System.out.println(obj7b.getDate());
  }
}

public final class Class6 implements Cloneable { // UndCC_Violation - clone is not properly implemented
  private Date date;

  public Class6(Date d) {
    this.date = new Date(d.getTime());
  }

  public Date getDate() {
    return (Date) date.clone();
  }

  public Object clone() throws CloneNotSupportedException { // mutable Date object not manually copied
    final Class6 cloned = (Class6) super.clone();
    return cloned;
  }
}

public class Class7 implements Cloneable { // UndCC_Violation - class is nonfinal
  private Date date;

  public Class7(Date d) {
    this.date = new Date(d.getTime());
  }

  public Date getDate() {
    return (Date) date.clone();
  }

  public void setDate(Date d) {
    this.date = (Date) d.clone();
  }

  public Object clone() throws CloneNotSupportedException { // clone method
    final Class7 cloned = (Class7) super.clone();
    cloned.date = (Date) date.clone();
    return cloned;
  }
}
