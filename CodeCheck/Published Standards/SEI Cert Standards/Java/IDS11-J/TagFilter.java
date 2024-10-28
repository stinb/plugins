import java.text.Normalizer;
import java.text.Normalizer.Form;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
 
public class TagFilter {
  
  public static String filterString(String str) {
    String s = Normalizer.normalize(str, Form.NFKC);
 
    // Replaces all noncharacter code points with Unicode U+FFFD
    s = s.replaceAll("[\\p{Cn}]", "\uFFFD");
 
    // Validate input
    Pattern pattern = Pattern.compile("<script>");
    Matcher matcher = pattern.matcher(s);
    if (matcher.find()) {
      throw new IllegalArgumentException("Invalid input");
    }
    return s;
  }
  public static void main(String[] args) {
    // "\uFDEF" is a non-character code point
    String maliciousInput = "<scr" + "\uFDEF" + "ipt>";
    String s = filterString(maliciousInput);
    // s = <scr?ipt>
  }
}
