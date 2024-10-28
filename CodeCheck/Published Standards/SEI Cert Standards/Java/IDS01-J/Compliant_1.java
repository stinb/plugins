import java.text.Normalizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Compliant_1 {
    public static void main(String argsp[]) {
        String s = "\uFE64" + "script" + "\uFE65";

        // Normalize
        s = Normalizer.normalize(s, Normalizer.Form.NFKC);

        // UndCC_Validate
        Pattern pattern = Pattern.compile("[<>]");
        Matcher matcher = pattern.matcher(s);
        if (matcher.find()) {
            // Found blacklisted tag
            throw new IllegalStateException();
        } else {
            // ...
        }
    }
}
