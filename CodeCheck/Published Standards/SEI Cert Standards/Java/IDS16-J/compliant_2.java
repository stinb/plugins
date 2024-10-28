import java.io.BufferedOutputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.StringReader;

import javax.xml.XMLConstants;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;

import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;

public class OnlineStore3 {
  
  private static void createXMLStream(final BufferedOutputStream outStream,
                                      final String quantity) throws IOException {
    String xmlString;
    xmlString = "<item>\n<description>Widget</description>\n"
                + "<price>500.0</price>\n" + "<quantity>" + quantity
                + "</quantity></item>";
    InputSource xmlStream = new InputSource(new StringReader(xmlString));
    // Build a validating SAX parser using our schema
    SchemaFactory sf = SchemaFactory
                       .newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
    DefaultHandler defHandler = new DefaultHandler() {
      public void warning(SAXParseException s) throws SAXParseException {
        throw s;
      }
      public void error(SAXParseException s) throws SAXParseException {
        throw s;
      }
      public void fatalError(SAXParseException s) throws SAXParseException {
        throw s;
      }
    };
    StreamSource ss = new StreamSource(new File("schema.xsd"));
    try {
      Schema schema = sf.newSchema(ss);
      SAXParserFactory spf = SAXParserFactory.newInstance();
      spf.setSchema(schema);
      SAXParser saxParser = spf.newSAXParser();
      // To set the custom entity resolver,
      // an XML reader needs to be created
      XMLReader reader = saxParser.getXMLReader();
      reader.setEntityResolver(new CustomResolver());
      saxParser.parse(xmlStream, defHandler);
    } catch (ParserConfigurationException x) {
      throw new IOException("Unable to validate XML", x);
    } catch (SAXException x) {
      throw new IOException("Invalid quantity", x);
    }
    // Our XML is valid, proceed
    outStream.write(xmlString.getBytes());
    outStream.flush();
  }
}
