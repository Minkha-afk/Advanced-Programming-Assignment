import java.util.ArrayList;
import java.util.List;

abstract class Account {

    private final String accountNumber;
    private final String ownerName;
    private double balance;

    public Account(String accountNumber, String ownerName, double initialBalance) {
        if (accountNumber == null || accountNumber.isBlank())
            throw new IllegalArgumentException("Account number cannot be blank.");
        if (ownerName == null || ownerName.isBlank())
            throw new IllegalArgumentException("Owner name cannot be blank.");
        if (initialBalance < 0)
            throw new IllegalArgumentException("Initial balance cannot be negative.");

        this.accountNumber = accountNumber;
        this.ownerName = ownerName;
        this.balance = initialBalance;
    }

    public Account(String accountNumber, String ownerName) {
        this(accountNumber, ownerName, 0.0);
    }

    public String getAccountNumber() { return accountNumber; }
    public String getOwnerName() { return ownerName; }
    public double getBalance() { return balance; }

    protected void setBalance(double balance) { this.balance = balance; }

    public void deposit(double amount) {
        if (amount <= 0)
            throw new IllegalArgumentException(
                "Deposit amount must be positive. Got: " + amount);

        balance += amount;
        System.out.printf("  [+] Deposited ₹%.2f  →  New balance: ₹%.2f%n",
                          amount, balance);
    }

    public void withdraw(double amount) {
        if (amount <= 0)
            throw new IllegalArgumentException(
                "Withdrawal amount must be positive. Got: " + amount);
        if (amount > balance)
            throw new IllegalStateException(
                String.format("Insufficient funds. Balance: ₹%.2f, Requested: ₹%.2f",
                              balance, amount));

        balance -= amount;
        System.out.printf("  [-] Withdrew  ₹%.2f  →  New balance: ₹%.2f%n",
                          amount, balance);
    }

    public void display() {
        System.out.println("  Account No : " + accountNumber);
        System.out.println("  Owner      : " + ownerName);
        System.out.printf ("  Balance    : ₹%.2f%n", balance);
    }
}
    
class SavingsAccount extends Account {

    private double interestRate;

    public SavingsAccount(String accountNumber, String ownerName, double initialBalance, double interestRate) {
        super(accountNumber, ownerName, initialBalance);
        setInterestRate(interestRate);
    }

    public SavingsAccount(String accountNumber, String ownerName, double initialBalance) {
        this(accountNumber, ownerName, initialBalance, 0.035);
    }

    public void setInterestRate(double rate) {
        if (rate < 0 || rate > 1)
            throw new IllegalArgumentException(
                "Interest rate must be between 0 and 1 (e.g. 0.06 for 6%). Got: " + rate);
        this.interestRate = rate;
    }

    public double getInterestRate() { return interestRate; }

    public void applyInterest() {
        double interest = getBalance() * interestRate;
        deposit(interest);
        System.out.printf("  [i] Interest applied at %.1f%%: ₹%.2f%n",
                          interestRate * 100, interest);
    }

    @Override
    public void display() {
        System.out.println("┌─── SAVINGS ACCOUNT ──────────────────────────");
        super.display();
        System.out.printf ("  Interest   : %.1f%%%n", interestRate * 100);
        System.out.printf ("  Projected  : ₹%.2f  (after 1 year)%n",
                           getBalance() * (1 + interestRate));
        System.out.println("└──────────────────────────────────────────────");
    }
}

class CurrentAccount extends Account {

    private final double overdraftLimit;

    public CurrentAccount(String accountNumber, String ownerName,
                          double initialBalance, double overdraftLimit) {
        super(accountNumber, ownerName, initialBalance);
        if (overdraftLimit < 0)
            throw new IllegalArgumentException("Overdraft limit cannot be negative.");
        this.overdraftLimit = overdraftLimit;
    }

    public CurrentAccount(String accountNumber, String ownerName,
                          double initialBalance) {
        this(accountNumber, ownerName, initialBalance, 0);
    }

    public double getOverdraftLimit() { return overdraftLimit; }

    @Override
    public void withdraw(double amount) {
        if (amount <= 0)
            throw new IllegalArgumentException(
                "Withdrawal amount must be positive. Got: " + amount);

        double minAllowed = -overdraftLimit;
        if ((getBalance() - amount) < minAllowed)
            throw new IllegalStateException(
                String.format(
                    "Exceeds overdraft limit. Balance: ₹%.2f, Limit: ₹%.2f, Requested: ₹%.2f",
                    getBalance(), overdraftLimit, amount));

        setBalance(getBalance() - amount);
        System.out.printf("  [-] Withdrew  ₹%.2f  →  New balance: ₹%.2f%n",
                          amount, getBalance());
    }

    @Override
    public void display() {
        System.out.println("┌─── CURRENT ACCOUNT ──────────────────────────");
        super.display();
        System.out.printf ("  Overdraft  : ₹%.2f available%n", overdraftLimit);
        System.out.printf ("  Available  : ₹%.2f  (balance + overdraft)%n",
                           getBalance() + overdraftLimit);
        System.out.println("└──────────────────────────────────────────────");
    }
}

public class BankingSystem {

    private static void attempt(String label, Runnable action) {
        try {
            System.out.println("  > " + label);
            action.run();
        } catch (IllegalArgumentException | IllegalStateException e) {
            System.out.println("  [!] Caught " + e.getClass().getSimpleName()
                               + ": " + e.getMessage());
        }
    }

    public static void main(String[] args) {

        System.out.println("\n╔══════════════════════════════════════════════╗");
        System.out.println("║           BANKING SYSTEM DEMO                ║");
        System.out.println("╚══════════════════════════════════════════════╝");

        SavingsAccount savings = new SavingsAccount("SAV-001", "Minha", 15000, 0.06);

        CurrentAccount current = new CurrentAccount("CUR-002", "Aryan", 5000, 10000);

        SavingsAccount savingsDefault = new SavingsAccount("SAV-003", "Priya", 8000);

        System.out.println("\n=== SavingsAccount Transactions ===");
        savings.deposit(5000);
        savings.withdraw(2000);
        savings.applyInterest();

        System.out.println("\n=== SavingsAccount Display ===");
        savings.display();

        System.out.println("\n=== CurrentAccount Transactions ===");
        current.deposit(3000);
        current.withdraw(7000);
        System.out.printf("  Balance after overdraft: ₹%.2f%n", current.getBalance());

        System.out.println("\n=== CurrentAccount Display ===");
        current.display();

        System.out.println("\n=== Validation: Bad Operations ===");

        attempt("Negative deposit",
                () -> savings.deposit(-500));

        attempt("Overdraft savings (no overdraft allowed)",
                () -> savings.withdraw(999999));

        attempt("Exceeds overdraft limit on CurrentAccount",
                () -> current.withdraw(999999));

        attempt("Invalid interest rate",
                () -> savingsDefault.setInterestRate(2.5));

        System.out.println("\n=== Polymorphism: Unified Account Roster ===");
        List<Account> bank = new ArrayList<>();
        bank.add(savings);
        bank.add(current);
        bank.add(savingsDefault);

        for (Account acc : bank) {
            acc.display();
            System.out.println();
        }

        assert savings.getBalance() > 0 : "Balance should be positive after operations";
        System.out.println("  [✓] Assertion passed: balance is positive.");
    }
}
