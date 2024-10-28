import java.io.FileInputStream;
import java.io.IOException;
import java.nio.CharBuffer;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class LogSearch {
    public static void FindLogEntry(String search) {
        // Construct regex dynamically from user string
        String regex = "(.*? +public\\[\\d+\\] +.*" + search + ".*)"; // UndCC_Violation, unsanitized
        Pattern searchPattern = Pattern.compile(regex);
        try (FileInputStream fis = new FileInputStream("log.txt")) {
            FileChannel channel = fis.getChannel();
            // Get the file's size and map it into memory
            long size = channel.size();
            final MappedByteBuffer mappedBuffer = channel.map(
                    FileChannel.MapMode.READ_ONLY, 0, size);
            Charset charset = Charset.forName("ISO-8859-15");
            final CharsetDecoder decoder = charset.newDecoder();
            // Read file into char buffer
            CharBuffer log = decoder.decode(mappedBuffer);
            Matcher logMatcher = searchPattern.matcher(log);
            while (logMatcher.find()) {
                String match = logMatcher.group();
                if (!match.isEmpty()) {
                    System.out.println(match);
                }
            }
        } catch (IOException ex) {
            System.err.println("thrown exception: " + ex.toString());
            Throwable[] suppressed = ex.getSuppressed();
            for (int i = 0; i < suppressed.length; i++) {
                System.err.println("suppressed exception: "
                        + suppressed[i].toString());
            }
        }
        return;
    }
}