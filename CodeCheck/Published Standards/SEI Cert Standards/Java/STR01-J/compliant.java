

public class StringDemo {
  public static String trim1(String string) {
    int ch1;
    int i;
    for (i = 0; i < string.length(); i += Character.charCount(ch1)) {
      ch1 = string.codePointAt(i);
      if (!Character.isLetter(ch1)) {
        break;
      }
    }
    return string.substring(i);
  }
}
