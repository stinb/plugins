public class BankAccount {
// Private fields to store account details
private String accountNumber;
private String accountType;
private String accountOwnerName;
private String accountOwnerSSN;
private double balance;

// Default constructor
public BankAccount() {
}

// Setter methods for each field
public void setAccountNumber(String accountNumber) {
  this.accountNumber = accountNumber;
}

public void setAccountType(String accountType) {
  this.accountType = accountType;
}

public void setAccountOwnerName(String accountOwnerName) {
  this.accountOwnerName = accountOwnerName;
}

public void setAccountOwnerSSN(String accountOwnerSSN) {
  this.accountOwnerSSN = accountOwnerSSN;
}

public void setBalance(double balance) {
  this.balance = balance;
}

// Optional: Getter methods for completeness, though not required for compilation
public String getAccountNumber() {
  return accountNumber;
}

public String getAccountType() {
  return accountType;
}

public String getAccountOwnerName() {
  return accountOwnerName;
}

public String getAccountOwnerSSN() {
  return accountOwnerSSN;
}

public double getBalance() {
  return balance;
}
}

// Class containing the createBankAccount method
public class BankAccountCreator {
  public BankAccount createBankAccount(String accountNumber, String accountType,
                                       String accountName, String accountSSN, double balance) {
    
    BankAccount account = new BankAccount();
    account.setAccountNumber(accountNumber); // UndCC_Violation
    account.setAccountType(accountType); // UndCC_Violation
    account.setAccountOwnerName(accountName); // UndCC_Violation
    account.setAccountOwnerSSN(accountSSN); // UndCC_Violation
    account.setBalance(balance); // UndCC_Violation
    
    return account;
  }
}

