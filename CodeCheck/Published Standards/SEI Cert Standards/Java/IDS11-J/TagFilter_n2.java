import java.text.Normalizer;
import java.text.Normalizer.Form;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
  
public class TagFilter_n2 {
  public static void validateInput(String str) {
    // Validate input
    Pattern pattern = Pattern.compile("<script>");
    Matcher matcher = pattern.matcher(str);
    if (matcher.find()) {
      throw new IllegalArgumentException("Invalid input");
    }
  }
  
  public static String cleanup(String str) {
    // Deletes noncharacter code points
    str = str.replaceAll("[\\p{Cn}]", "");  // UndCC_FalseNeg - validation and cleanup are done in separate methods, cannot confirm that 
                                            // string modification was performed before validation
    return str;
  }
  
  public static String filterString(String str) {
    String s = Normalizer.normalize(str, Form.NFKC);
    validateInput(s);
    s = cleanup(s);
    return s;
  }
 
  public static void main(String[] args) {
    // "\uFDEF" is a noncharacter code point
    String maliciousInput = "<scr" + "\uFDEF" + "ipt>";
    String sb = filterString(maliciousInput);
    System.out.print(sb);
    // sb = "<script>"
  }
}
