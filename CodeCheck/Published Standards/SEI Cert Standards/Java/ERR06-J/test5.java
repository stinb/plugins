// Noncompliant Code Example (Thread.stop(Throwable))

public class Test5 {
  static void sneakyThrow(Throwable t) {
    Thread.currentThread().stop(t); // UndCC_Violation - undeclared & uncaught Throwable
  }
}
