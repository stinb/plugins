import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;

public class OnlineStore1 {
  private static void createXMLStreamBad(final BufferedOutputStream outStream,
                                         final String quantity) throws IOException {
    String xmlString = "<item>\n<description>Widget</description>\n"
                       + "<price>500</price>\n" + "<quantity>" + quantity  // UndCC_Violation
                       + "</quantity></item>";
    outStream.write(xmlString.getBytes());
    outStream.flush();
  }
}
