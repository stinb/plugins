import java.util.*;
import java.util.concurrent.atomic.*;
import java.util.concurrent.locks.*;


final class Main {
  public static void main(String[] args) {
    BankAccount0 a = new BankAccount0(5000);
    BankAccount0 b = new BankAccount0(6000);
    BankAccount0.initiateTransfer(a, b, 1000); // starts thread 1
    BankAccount0.initiateTransfer(b, a, 1000); // starts thread 2
  }
}


// Noncompliant Code Example (Different Lock Orders)
final class BankAccount0 {
  private double balanceAmount;  // Total amount in bank account

  BankAccount0(double balance) {
    this.balanceAmount = balance;
  }

  // Deposits the amount from this object instance
  // to BankAccount0 instance argument ba
  private void depositAmount(BankAccount0 ba, double amount) {
    synchronized (this) {
      synchronized (ba) { // UndCC_Violation
        if (amount > balanceAmount) {
          throw new IllegalArgumentException(
               "Transfer cannot be completed"
          );
        }
        ba.balanceAmount += amount;
        this.balanceAmount -= amount;
      }
    }
  }

  public static void initiateTransfer(final BankAccount0 first,
    final BankAccount0 second, final double amount) {

    Thread transfer = new Thread(new Runnable() {
        public void run() {
          first.depositAmount(second, amount);
        }
    });
    transfer.start();
  }
}


// Compliant Solution (Private Static Final Lock Object)
final class BankAccount1 {
  private double balanceAmount;  // Total amount in bank account
  private static final Object lock = new Object();

  BankAccount1(double balance) {
    this.balanceAmount = balance;
  }

  // Deposits the amount from this object instance
  // to BankAccount1 instance argument ba
  private void depositAmount(BankAccount1 ba, double amount) {
    synchronized (lock) { // UndCC_Valid
      if (amount > balanceAmount) {
        throw new IllegalArgumentException(
            "Transfer cannot be completed");
      }
      ba.balanceAmount += amount;
      this.balanceAmount -= amount;
    }
  }

  public static void initiateTransfer(final BankAccount1 first,
    final BankAccount1 second, final double amount) {

    Thread transfer = new Thread(new Runnable() {
        @Override public void run() {
          first.depositAmount(second, amount);
        }
    });
    transfer.start();
  }
}


// Compliant Solution (Ordered Locks)
final class BankAccount2 implements Comparable<BankAccount2> {
  private double balanceAmount;  // Total amount in bank account
  private final Object lock;

  private final long id; // Unique for each BankAccount2
  private static final AtomicLong nextID = new AtomicLong(0); // Next unused ID

  BankAccount2(double balance) {
    this.balanceAmount = balance;
    this.lock = new Object();
    this.id = nextID.getAndIncrement();
  }

  @Override public int compareTo(BankAccount2 ba) {
     return (this.id > ba.id) ? 1 : (this.id < ba.id) ? -1 : 0;
  }

  // Deposits the amount from this object instance
  // to BankAccount2 instance argument ba
  public void depositAmount(BankAccount2 ba, double amount) {
    BankAccount2 former, latter;
    if (compareTo(ba) < 0) {
      former = this;
      latter = ba;
    } else {
      former = ba;
      latter = this;
    }
    synchronized (former) {   // UndCC_Valid
      synchronized (latter) { // UndCC_Valid
        if (amount > balanceAmount) {
          throw new IllegalArgumentException(
              "Transfer cannot be completed");
        }
        ba.balanceAmount += amount;
        this.balanceAmount -= amount;
      }
    }
  }

  public static void initiateTransfer(final BankAccount2 first,
    final BankAccount2 second, final double amount) {

    Thread transfer = new Thread(new Runnable() {
        @Override public void run() {
          first.depositAmount(second, amount);
        }
    });
    transfer.start();
  }
}


// Compliant Solution (ReentrantLock)
final class BankAccount3 {
  private double balanceAmount;  // Total amount in bank account
  private final Lock lock = new ReentrantLock();
  private final Random number = new Random(123L);

  BankAccount3(double balance) {
    this.balanceAmount = balance;
  }

  // Deposits amount from this object instance
  // to BankAccount3 instance argument ba
  private void depositAmount(BankAccount3 ba, double amount)
                             throws InterruptedException {
    while (true) {
      if (this.lock.tryLock()) {
        try {
          if (ba.lock.tryLock()) {
            try {
              if (amount > balanceAmount) {
                throw new IllegalArgumentException(
                    "Transfer cannot be completed");
              }
              ba.balanceAmount += amount;
              this.balanceAmount -= amount;
              break;
            } finally {
              ba.lock.unlock();
            }
          }
        } finally {
          this.lock.unlock();
        }
      }
      int n = number.nextInt(1000);
      int TIME = 1000 + n; // 1 second + random delay to prevent livelock
      Thread.sleep(TIME);
    }
  }

  public static void initiateTransfer(final BankAccount3 first,
    final BankAccount3 second, final double amount) {

    Thread transfer = new Thread(new Runnable() {
        public void run() {
          try {
            first.depositAmount(second, amount);
          } catch (InterruptedException e) {
            Thread.currentThread().interrupt(); // Reset interrupted status
          }
        }
    });
    transfer.start();
  }
}


// Immutable WebRequest
public final class WebRequest {
  private final long bandwidth;
  private final long responseTime;

  public WebRequest(long bandwidth, long responseTime) {
    this.bandwidth = bandwidth;
    this.responseTime = responseTime;
  }

  public long getBandwidth() {
    return bandwidth;
  }

  public long getResponseTime() {
    return responseTime;
  }
}


// Noncompliant Code Example (Different Lock Orders, Recursive)
public final class WebRequestAnalyzer0 {
  private final Vector<WebRequest> requests = new Vector<WebRequest>();

  public boolean addWebRequest(WebRequest request) {
    return requests.add(new WebRequest(request.getBandwidth(),
                        request.getResponseTime()));
  }

  public double getAverageBandwidth() {
    if (requests.size() == 0) {
      throw new IllegalStateException("The vector is empty!");
    }
    return calculateAverageBandwidth(0, 0);
  }

  public double getAverageResponseTime() {
    if (requests.size() == 0) {
      throw new IllegalStateException("The vector is empty!");
    }
    return calculateAverageResponseTime(requests.size() - 1, 0);
  }

  private double calculateAverageBandwidth(int i, long bandwidth) {
    if (i == requests.size()) {
      return bandwidth / requests.size();
    }
    synchronized (requests.elementAt(i)) {
      bandwidth += requests.get(i).getBandwidth();
      // Acquires locks in increasing order
      return calculateAverageBandwidth(++i, bandwidth);
    }
  }

  private double calculateAverageResponseTime(int i, long responseTime) {
    if (i <= -1) {
      return responseTime / requests.size();
    }
    synchronized (requests.elementAt(i)) {
      responseTime += requests.get(i).getResponseTime();
      // Acquires locks in decreasing order
      return calculateAverageResponseTime(--i, responseTime); // UndCC_Violation
    }
  }
}


// Compliant Solution
public final class WebRequestAnalyzer1 {
  private final Vector<WebRequest> requests = new Vector<WebRequest>();

  public boolean addWebRequest(WebRequest request) {
    return requests.add(new WebRequest(request.getBandwidth(),
                        request.getResponseTime()));
  }

  public double getAverageBandwidth() {
    if (requests.size() == 0) {
      throw new IllegalStateException("The vector is empty!");
    }
    return calculateAverageBandwidth(0, 0);
  }

  public double getAverageResponseTime() {
    if (requests.size() == 0) {
      throw new IllegalStateException("The vector is empty!");
    }
    return calculateAverageResponseTime(0, 0);
  }

  private double calculateAverageBandwidth(int i, long bandwidth) {
    if (i == requests.size()) {
      return bandwidth / requests.size();
    }
    synchronized (requests.elementAt(i)) {
      // Acquires locks in increasing order
      bandwidth += requests.get(i).getBandwidth();
      return calculateAverageBandwidth(++i, bandwidth);       // UndCC_Valid
    }
  }

  private double calculateAverageResponseTime(int i, long responseTime) {
    if (i == requests.size()) {
      return responseTime / requests.size();
    }
    synchronized (requests.elementAt(i)) {
      // Acquires locks in increasing order
      responseTime += requests.get(i).getResponseTime();
      return calculateAverageResponseTime(++i, responseTime); // UndCC_Valid
    }
  }
}
