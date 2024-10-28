import java.net.*;
import java.util.*;


final class Book {
  // Could change its locking policy in the future
  // to use private final locks
  private final String title;
  private Calendar dateIssued;
  private Calendar dateDue;

  Book(String title) {
    this.title = title;
  }

  public synchronized void issue(int days) {  // UndCC_Valid
    dateIssued = Calendar.getInstance();
    dateDue = Calendar.getInstance();
    dateDue.add(dateIssued.DATE, days);
  }

  public synchronized Calendar getDueDate() { // UndCC_Valid
    return dateDue;
  }
}


// Noncompliant Code Example (Intrinsic Lock)
public class BookWrapper0 {
  private final Book book;

  BookWrapper0(Book book) {
    this.book = book;
  }

  public void issue(int days) {
    book.issue(days);
  }

  public Calendar getDueDate() {
    return book.getDueDate();
  }

  public void renew() {
    synchronized(book) { // UndCC_Violation - syncronizing on thread-safe instance
      if (book.getDueDate().before(Calendar.getInstance())) {
        throw new IllegalStateException("Book overdue");
      } else {
        book.issue(14); // Issue book for 14 days
      }
    }
  }
}


// Compliant Solution (Private Final Lock Object)
public final class BookWrapper1 {
  private final Book book;
  private final Object lock = new Object();

  BookWrapper1(Book book) {
    this.book = book;
  }

  public void issue(int days) {
    synchronized(lock) { // UndCC_Valid
      book.issue(days);
    }
  }

  public Calendar getDueDate() {
    synchronized(lock) { // UndCC_Valid
      return book.getDueDate();
    }
  }

  public void renew() {
    synchronized(lock) { // UndCC_Valid
      if (book.getDueDate().before(Calendar.getInstance())) {
        throw new IllegalStateException("Book overdue");
      } else {
        book.issue(14); // Issue book for 14 days
      }
    }
  }
}


// This class could change its locking policy in the future,
// for example, if new non-atomic methods are added
class IPAddressList {
  private final List<InetAddress> ips =
      Collections.synchronizedList(new ArrayList<InetAddress>());

  public List<InetAddress> getList() {
    return ips; // No defensive copies required
                // as visibility is package-private
  }

  public void addIPAddress(InetAddress address) {
    ips.add(address);
  }
}


// Noncompliant Code Example (Class Extension and Accessible Member Lock)
class PrintableIPAddressList0 extends IPAddressList {
  public void addAndPrintIPAddresses(InetAddress address) {
    synchronized (getList()) { // UndCC_Violation - syncronizing on member of thread-safe superclass
      addIPAddress(address);
      InetAddress[] ia =
          (InetAddress[]) getList().toArray(new InetAddress[0]);
      // ...
    }
  }
}


// Compliant Solution (Composition)
// Class IPAddressList remains unchanged
class PrintableIPAddressList1 {
  private final IPAddressList ips;

  public PrintableIPAddressList1(IPAddressList list) {
    this.ips = list;
  }

  public synchronized void addIPAddress(InetAddress address) { // UndCC_Valid
    ips.addIPAddress(address);
  }

  public synchronized void addAndPrintIPAddresses(InetAddress address) { // UndCC_Valid
    addIPAddress(address);
    InetAddress[] ia =
        (InetAddress[]) ips.getList().toArray(new InetAddress[0]);
    // ...
  }
}
