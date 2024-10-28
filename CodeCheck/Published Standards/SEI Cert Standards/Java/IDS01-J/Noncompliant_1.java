import java.text.Normalizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Noncompliant_1 {
    public static void main(String argsp[]) {
        // String s may be user controllable
        // \uFE64 is normalized to < and \uFE65 is normalized to > using the NFKC
        // normalization form
        String s = "\uFE64" + "script" + "\uFE65";

        // UndCC_Validate
        Pattern pattern = Pattern.compile("[<>]"); // Check for angle brackets
        Matcher matcher = pattern.matcher(s);

        if (matcher.find()) {
            // Found black listed tag
            throw new IllegalStateException();
        } else {
            // ...
        }

        // String is never normalized
    }
}
