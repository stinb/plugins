import java.util.regex.Pattern;

public class Compliant {
    public static void FindLogEntry(String search) {
        // Sanitize search string
        StringBuilder sb = new StringBuilder(search.length());
        for (int i = 0; i < search.length(); ++i) {
            char ch = search.charAt(i);
            if (Character.isLetterOrDigit(ch) || ch == ' ' || ch == '\'') {
                sb.append(ch);
            }
        }
        search = sb.toString();

        // Construct regex dynamically from user string
        String regex = "(.*? +public\\[\\d+\\] +.*" + search + ".*)"; // UndCC_FalsePos, false positive - see Notes section
        Pattern searchPattern = Pattern.compile(regex);
        // ...
    }

    public static void FindLogEntry_2(String search) {
        // Sanitize search string
        search = Pattern.quote(search);
        // Construct regex dynamically from user string
        String regex = "(.*? +public\\[\\d+\\] +.*" + search + ".*)"; // UndCC_Valid, 'search' string sanitized using
                                                                      // quote()
        Pattern searchPattern = Pattern.compile(regex);
        // ...
    }

}
