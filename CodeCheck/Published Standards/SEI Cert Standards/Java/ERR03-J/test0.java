class Dimensions {
  private int length;
  private int width;
  private int height;
  static public final int PADDING = 2;
  static public final int MAX_DIMENSION = 10;

  public Dimensions(int length, int width, int height) {
    this.length = length;
    this.width = width;
    this.height = height;
  }

  // Noncompliant Code Example
  protected int getVolumePackage0(int weight) {
    length += PADDING; // UndCC_Violation(0) - Modification before failure
    width  += PADDING; // UndCC_Violation(0) - Modification before failure
    height += PADDING; // UndCC_Violation(0) - Modification before failure
    try {
      if (length <= PADDING || width <= PADDING || height <= PADDING ||
        length > MAX_DIMENSION + PADDING || width > MAX_DIMENSION + PADDING ||
        height > MAX_DIMENSION + PADDING || weight <= 0 || weight > 20) {
        throw new IllegalArgumentException(); // UndCC_Violation
      }

      int volume = length * width * height;
      length -= PADDING; width -= PADDING; height -= PADDING; // Revert
      return volume;
    } catch (Throwable t) {
      MyExceptionReporter mer = new MyExceptionReporter();
      mer.report(t); // Sanitize
      return -1; // Non-positive error code
    }
  }

  // Compliant Solution (Rollback)
  protected int getVolumePackage1(int weight) {
    length += PADDING;
    width  += PADDING;
    height += PADDING;
    try {
      if (length <= PADDING || width <= PADDING || height <= PADDING ||
        length > MAX_DIMENSION + PADDING || width > MAX_DIMENSION + PADDING ||
        height > MAX_DIMENSION + PADDING || weight <= 0 || weight > 20) {
        throw new IllegalArgumentException(); // UndCC_Valid
      }

      int volume = length * width * height;
      length -= PADDING; width -= PADDING; height -= PADDING; // Revert
      return volume;
    } catch (Throwable t) {
      MyExceptionReporter mer = new MyExceptionReporter();
      mer.report(t); // Sanitize
      length -= PADDING; width -= PADDING; height -= PADDING; // Revert
      return -1; // Non-positive error code
    }
  }

  // Compliant Solution (finally Clause)
  protected int getVolumePackage2(int weight) {
    length += PADDING;
    width  += PADDING;
    height += PADDING;
    try {
      if (length <= PADDING || width <= PADDING || height <= PADDING ||
        length > MAX_DIMENSION + PADDING ||
        width > MAX_DIMENSION + PADDING ||
        height > MAX_DIMENSION + PADDING ||
        weight <= 0 || weight > 20) {
        throw new IllegalArgumentException(); // UndCC_Valid
      }

      int volume = length * width * height;
      return volume;
    } catch (Throwable t) {
      MyExceptionReporter mer = new MyExceptionReporter();
      mer.report(t); // Sanitize
      return -1; // Non-positive error code
    } finally {
      // Revert
      length -= PADDING; width -= PADDING; height -= PADDING;
    }
  }

  // Compliant Solution (Input Validation)
  protected int getVolumePackage3(int weight) {
    try {
      if (length <= 0 || width <= 0 || height <= 0 ||
          length > MAX_DIMENSION || width > MAX_DIMENSION || height > MAX_DIMENSION ||
          weight <= 0 || weight > 20) {
        throw new IllegalArgumentException(); // UndCC_Valid - Validate first
      }
    } catch (Throwable t) { MyExceptionReporter mer = new MyExceptionReporter();
      mer.report(t); // Sanitize
      return -1;
    }

    length += PADDING;
    width  += PADDING;
    height += PADDING;

    int volume = length * width * height;
    length -= PADDING; width -= PADDING; height -= PADDING;
    return volume;
  }

  // Compliant Solution (Unmodified Object)
  protected int getVolumePackage4(int weight) {
    try {
      if (length <= 0 || width <= 0 || height <= 0 ||
          length > MAX_DIMENSION || width > MAX_DIMENSION ||
          height > MAX_DIMENSION || weight <= 0 || weight > 20) {
        throw new IllegalArgumentException(); // UndCC_Valid - Validate first
      }
    } catch (Throwable t) {
      MyExceptionReporter mer = new MyExceptionReporter();
      mer.report(t); // Sanitize
      return -1;
    }

    int volume = (length + PADDING) * (width + PADDING) *
                (height + PADDING);
    return volume;
  }

  public static void main(String[] args) {
    Dimensions d = new Dimensions(8, 8, 8);
    System.out.println(d.getVolumePackage0(21)); // Prints -1 (error)
    System.out.println(d.getVolumePackage0(19));
    // Prints 1728 (12x12x12) instead of 1000 (10x10x10)
  }
}

class MyExceptionReporter {
  public static void report(Throwable t) {

  }
}
