import java.util.*;
import java.util.concurrent.atomic.*;
import java.util.concurrent.locks.*;

final class OffshoreAccount {
  private double dirtyMoney;  // Total account balance

  OffshoreAccount(double funds) {
    this.dirtyMoney = funds;
  }

  // Deposits the amount from this object instance to OffshoreAccount instance argument oa
  private void dumpFunds(OffshoreAccount oa, double amount) {
    synchronized (this) {
      synchronized (oa) { // UndCC_Violation
        if (amount > dirtyMoney) {
          throw new IllegalArgumentException(
               "Transfer cannot be completed"
          );
        }
        oa.dirtyMoney += amount;
        this.dirtyMoney -= amount;
      }
    }
  }

  public static void initiateTransfer(final OffshoreAccount first,
    final OffshoreAccount second, final double amount) {

    Thread transfer = new Thread(new Runnable() {
        public void run() {
          first.dumpFunds(second, amount);
        }
    });
    transfer.start();
  }
}
