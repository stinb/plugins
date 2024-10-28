import java.io.FileInputStream;
import java.io.IOException;
import java.nio.CharBuffer;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class emailInput {
  public static void validateEmailInput(String domain) {
    // Construct regex dynamically from user string
    String regex = "(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|" + domain; // UndCC_Violation, unsanitized
    Pattern emailPattern = Pattern.compile(regex);
  
    // ...
    
    return;
  }
}
