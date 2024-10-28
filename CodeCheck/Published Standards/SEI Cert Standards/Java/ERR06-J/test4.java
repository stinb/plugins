// Noncompliant Code Example (Generic Exception)

import java.io.*;

public class UndeclaredGen {
  static void undeclaredThrow0() throws RuntimeException {
    @SuppressWarnings("unchecked")  // Suppresses warnings
    Thr<RuntimeException> thr = (Thr<RuntimeException>)(Thr)
      new Thr<IOException>() {
        public void fn() throws IOException {
          throw new IOException();
        }
      };
    thr.fn();
  }

  static void undeclaredThrow1() throws RuntimeException {
    @SuppressWarnings("unchecked")  // Suppresses warnings
    Thr<RuntimeException> thr = (Thr)
      new Thr<IOException>() {
        public void fn() throws IOException {
          throw new IOException();
        }
      };
    thr.fn();
  }

  public static void main(String[] args) {
    undeclaredThrow0(); // UndCC_Violation - casting EXC extends Exception
    undeclaredThrow1(); // UndCC_Violation - casting EXC extends Exception
  }
}

interface Thr<EXC extends Exception> {
  void fn() throws EXC;
}
