final class Foo2 {
  private volatile Helper helper;
 
  public Helper getHelper() {
    return helper;
  }
 
  public void setHelper(int num) {
    helper = new Helper(num);
  }
}