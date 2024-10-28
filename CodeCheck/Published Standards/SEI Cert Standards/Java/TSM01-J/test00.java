import java.util.logging.Level;
import java.util.logging.Logger;


// Publish before Initialization


final class Publisher0 {
  public static volatile Publisher0 published;
  int num;

  Publisher0(int number) {
    published = this; // UndCC_Violation
    // assignment with 'this' before initialization
    this.num = number;
  }
}


// Nonvolatile Public Static Field


final class Publisher1 {
  public static Publisher1 published;
  int num;

  Publisher1(int number) {
    this.num = number;
    published = this; // UndCC_Violation
    // assignment to nonvolatile and nonfinal and public variable with 'this'
  }
}


// Volatile Field and Publish after Initialization


final class Publisher2 {
  static volatile Publisher2 published;
  int num;

  Publisher2(int number) {
    this.num = number;
    published = this; // UndCC_Valid
    // assignment to (volatile or final) and nonpublic variable with 'this'
  }
}


// Public Static Factory Method


final class Publisher3 {
  final int num;

  private Publisher3(int number) { // UndCC_Valid
    this.num = number;
  }

  public static Publisher3 newInstance(int number) {
    Publisher3 published = new Publisher3(number);
    return published;
  }
}


// Handlers


public interface ExceptionReporter {
  public void setExceptionReporter(ExceptionReporter er);
  public void report(Throwable exception);
}


public class DefaultExceptionReporter0 implements ExceptionReporter {
  public DefaultExceptionReporter0(ExceptionReporter er) {
    er.setExceptionReporter(this); // UndCC_Violation
    // superclass or superinterface uses 'this' in constructor
  }

  public void setExceptionReporter(ExceptionReporter er) {}
  public void report(Throwable exception) {}
}
public class MyExceptionReporter0 extends DefaultExceptionReporter0 {
  private final Logger logger;

  public MyExceptionReporter0(ExceptionReporter er) {
    super(er);
    logger = Logger.getLogger("com.organization.Log");
  }

  public void report(Throwable t) {
    logger.log(Level.FINEST,"Loggable exception occurred", t);
  }
}


public class DefaultExceptionReporter1 implements ExceptionReporter {
  public DefaultExceptionReporter1(ExceptionReporter er) {}

  public void publishExceptionReporter() {
    setExceptionReporter(this); // UndCC_Valid
    // superclass or superinterface uses 'this' outside constructor
  }

  public void setExceptionReporter(ExceptionReporter er) {}
  public void report(Throwable exception) {}
}
public class MyExceptionReporter1 extends DefaultExceptionReporter1 {
  private final Logger logger;

  public MyExceptionReporter1(ExceptionReporter er) {
    super(er);
    logger = Logger.getLogger("com.organization.Log");
  }
}


// Inner Class


public class DefaultExceptionReporter2 implements ExceptionReporter {
  public DefaultExceptionReporter2(ExceptionReporter er) {
    er.setExceptionReporter(new ExceptionReporter() { // UndCC_Violation
    // anonymous class published in nonprivate constructor
        public void report(Throwable t) {}
        public void setExceptionReporter(ExceptionReporter er) {}
    });
  }
  public void setExceptionReporter(ExceptionReporter er) {}
  public void report(Throwable exception) {}
}


public class DefaultExceptionReporter3 implements ExceptionReporter {
  private final ExceptionReporter defaultER;

  private DefaultExceptionReporter3(ExceptionReporter excr) {
    defaultER = new ExceptionReporter() { // UndCC_Valid
    // anonymous class created in a private constructor and published after construction
      public void report(Throwable t) {}
      public void setExceptionReporter(ExceptionReporter er) {}
    };
  }

  public static DefaultExceptionReporter3 newInstance(ExceptionReporter excr) {
    DefaultExceptionReporter3 der = new DefaultExceptionReporter3(excr);
    excr.setExceptionReporter(der.defaultER);
    return der;
  }

  public void setExceptionReporter(ExceptionReporter er) {}
  public void report(Throwable exception) {}
}


// Thread


final class ThreadStarter0 implements Runnable {
  public ThreadStarter0() {
    Thread thread = new Thread(this); // UndCC_Violation
    // 'this' used to start thread in constructor
    thread.start();
  }

  @Override public void run() {
    // ...
  }
}

final class ThreadStarter1 implements Runnable {
  public void startThread() {
    Thread thread = new Thread(this); // UndCC_Valid
    // 'this' to create and start thread after construction
    thread.start();
  }

  @Override public void run() {
    // ...
  }
}

final class ThreadStarter2 implements Runnable {
  Thread thread;

  public ThreadStarter2() {
    thread = new Thread(this); // UndCC_Valid
    // 'this' used to create thread in constructor, but not start it
  }

  public void startThread() {
    thread.start();
  }

  @Override public void run() {
    // ...
  }
}
