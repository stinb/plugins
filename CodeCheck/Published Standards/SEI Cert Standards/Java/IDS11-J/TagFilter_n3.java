import java.text.Normalizer;
import java.text.Normalizer.Form;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class SomeType {
  String stored_sb;
  SomeType(String sb) {
    stored_sb = sb;
  }
  void cleanup() {
    // Deletes noncharacter code points
    stored_sb = stored_sb.replaceAll("[\\p{Cn}]", "");  // UndCC_FalseNeg - input string is stored in an object after validation and the stored
                                                        // value is later modified. This behavior cannot be tracked with static analysis
  }
}
  
public class TagFilter_n3 {
  public static void validateInput(String str) {
    // Validate input
    Pattern pattern = Pattern.compile("<script>");
    Matcher matcher = pattern.matcher(str);
    if (matcher.find()) {
      throw new IllegalArgumentException("Invalid input");
    }
  }
  
  public static void main(String[] args) {
    // "\uFDEF" is a noncharacter code point
    String maliciousInput = "<scr" + "\uFDEF" + "ipt>";
    String s = Normalizer.normalize(maliciousInput, Form.NFKC);
    validateInput(s);
    
    // Store validated input in an object
    SomeType myobj = new SomeType(s);
    myobj.cleanup();
    
    System.out.print(myobj.stored_sb);
    // myobj.stored_sb = "<script>"
  }
}
