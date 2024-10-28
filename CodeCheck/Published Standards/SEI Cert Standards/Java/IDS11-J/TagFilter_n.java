import java.text.Normalizer;
import java.text.Normalizer.Form;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
  
public class TagFilter_n {
  public static String filterString(String str) {
    String s = Normalizer.normalize(str, Form.NFKC);
 
    // Validate input
    Pattern pattern = Pattern.compile("<script>");
    Matcher matcher = pattern.matcher(s);
    if (matcher.find()) {
      throw new IllegalArgumentException("Invalid input");
    }
 
    // Deletes noncharacter code points
    s = s.replaceAll("[\\p{Cn}]", "");  // UndCC_Violation
    return s;
  }
 
  public static void main(String[] args) {
    // "\uFDEF" is a noncharacter code point
    String maliciousInput = "<scr" + "\uFDEF" + "ipt>";
    String sb = filterString(maliciousInput);
    // sb = "<script>"
  }
}
