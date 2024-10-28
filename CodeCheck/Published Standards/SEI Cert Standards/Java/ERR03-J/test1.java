class Box {
  private int min;
  private int max;
  private int width;
  private int height;

  public Box(int width, int height) {
    this.min = 1;
    this.max = 1000;
    this.width = clamp(width);
    this.height = clamp(height);
  }

  protected int getWidth() {
    return this.width;
  }

  protected int getHeight() {
    return this.height;
  }

  protected void setWidth(int width) {
    this.width = clamp(width);
  }

  protected void setHeight(int height) {
    this.height = clampThrow(height); // UndCC_Valid - Throw before assignment
  }

  private int clamp(int value) {
    if (value < min)
      return min;
    if (value > max)
      return max;
    return value;
  }

  private int clampThrow(int value) {
    if (value < min || value > max)
      throw new IllegalArgumentException(); // UndCC_Valid - Object not changed
    return value;
  }

  public static void main(String[] args) {
    Box b = new Box(8, -1);
    try {
      b.setHeight(1001); // UndCC_Valid - Object not changed
    }
    catch (Throwable t) {

    }
    System.out.println(b.getHeight());
  }
}
