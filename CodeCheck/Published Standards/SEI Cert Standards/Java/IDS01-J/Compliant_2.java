import java.text.Normalizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Compliant_2 {
    public static void main(String argsp[]) {
        String mystring = "\uFE64" + "script" + "\uFE65";

        // Normalize
        mystring = Normalizer.normalize(mystring, Normalizer.Form.NFKC);
        
        // ...

        // UndCC_Validate
        Pattern pattern = Pattern.compile("[<>]");
        Matcher matcher = pattern.matcher(mystring);
        if (matcher.find()) {
            // Found blacklisted tag
            throw new IllegalStateException();
        } else {
            // ...
        }
    }
}
