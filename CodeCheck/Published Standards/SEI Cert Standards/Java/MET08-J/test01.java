public final class Main {
  public static void main(String[] args) {
    String s = "java";

    CaseInsensitiveString2 cis2 = new CaseInsensitiveString2("Java");
    System.out.println(cis2.equals(s)); // Returns true
    System.out.println(s.equals(cis2)); // Returns false
    System.out.println();

    CaseInsensitiveString3 cis3 = new CaseInsensitiveString3("Java");
    System.out.println(cis3.equals(s)); // Returns false
    System.out.println(s.equals(cis3)); // Returns false
    System.out.println();

    CaseInsensitiveString4 cis4 = new CaseInsensitiveString4("Java");
    System.out.println(cis4.equals(s)); // Returns false
    System.out.println(s.equals(cis4)); // Returns false
    System.out.println();

    CaseInsensitiveString5 cis5 = new CaseInsensitiveString5("Java");
    System.out.println(cis5.equals(s)); // Returns false
    System.out.println(s.equals(cis5)); // Returns false
    System.out.println();

    CaseInsensitiveString6 cis6 = new CaseInsensitiveString6("Java");
    System.out.println(cis6.equals(s)); // Returns false
    System.out.println(s.equals(cis6)); // Returns false
    System.out.println();

    CaseInsensitiveString7 cis7 = new CaseInsensitiveString7("Java");
    System.out.println(cis7.equals(s)); // Returns false
    System.out.println(s.equals(cis7)); // Returns false
    System.out.println();

    CaseInsensitiveString8 cis8 = new CaseInsensitiveString8("Java");
    System.out.println(cis8.equals(s)); // Returns false
    System.out.println(s.equals(cis8)); // Returns false
  }
}



public final class CaseInsensitiveString2 {
  private String s;

  public CaseInsensitiveString2(String s) {
    if (s == null) {
      throw new NullPointerException();
    }
    this.s = s;
  }

  public boolean equals(Object o) { // UndCC_Violation
    if (o instanceof CaseInsensitiveString2) {
      return s.equalsIgnoreCase(((CaseInsensitiveString2)o).s);
    }

    if (o instanceof String) {
      return s.equalsIgnoreCase((String)o);
    }
    return false;
  }
}



public final class CaseInsensitiveString3 {
  private String s;

  public CaseInsensitiveString3(String s) {
    if (s == null) {
      throw new NullPointerException();
    }
    this.s = s;
  }

  public boolean equals(Object o) { // UndCC_Valid
    return o instanceof CaseInsensitiveString3 &&
        ((CaseInsensitiveString3)o).s.equalsIgnoreCase(s);
  }
}



public final class CaseInsensitiveString4 {
  private String s;

  public CaseInsensitiveString4(String s) {
    if (s == null) {
      throw new NullPointerException();
    }
    this.s = s;
  }

  public boolean equals(Object o) { // UndCC_Valid
    if (o instanceof CaseInsensitiveString4) {
      return ((CaseInsensitiveString4)o).s.equalsIgnoreCase(s);
    }

    return false;
  }
}



public final class CaseInsensitiveString5 {
  private String s;

  public CaseInsensitiveString5(String s) {
    if (s == null) {
      throw new NullPointerException();
    }
    this.s = s;
  }

  public boolean equals(Object o) { // UndCC_Valid
    if (! (o instanceof CaseInsensitiveString5)) {
      return false;
    }

    return ((CaseInsensitiveString5)o).s.equalsIgnoreCase(s);
  }
}



public final class CaseInsensitiveString6 {
  private String s;

  public CaseInsensitiveString6(String s) {
    if (s == null) {
      throw new NullPointerException();
    }
    this.s = s;
  }

  public boolean equals(Object o) { // UndCC_Valid
    return o.getClass() == this.getClass() &&
        ((CaseInsensitiveString6)o).s.equalsIgnoreCase(s);
  }
}



public final class CaseInsensitiveString7 {
  private String s;

  public CaseInsensitiveString7(String s) {
    if (s == null) {
      throw new NullPointerException();
    }
    this.s = s;
  }

  public boolean equals(Object o) { // UndCC_Valid
    if (this.getClass() == o.getClass()) {
      return ((CaseInsensitiveString7)o).s.equalsIgnoreCase(s);
    }

    return false;
  }
}



public final class CaseInsensitiveString8 {
  private String s;

  public CaseInsensitiveString8(String s) {
    if (s == null) {
      throw new NullPointerException();
    }
    this.s = s;
  }

  public boolean equals(Object o) { // UndCC_Valid
    if (! (o.getClass() == this.getClass())) {
      return false;
    }

    return ((CaseInsensitiveString8)o).s.equalsIgnoreCase(s);
  }
}



public final class CaseInsensitiveString9 {
  private String s;

  public CaseInsensitiveString9(String s) {
    if (s == null) {
      throw new NullPointerException();
    }
    this.s = s;
  }

  public boolean equals(Object o) { // UndCC_Valid
    if (o.getClass() != this.getClass()) {
      return false;
    }

    return ((CaseInsensitiveString9)o).s.equalsIgnoreCase(s);
  }
}
