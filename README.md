# Assignment 9

## Question

Design a banking system in Java with:

A base class Account containing private fields: accountNumber, ownerName, balance
Provide getters/setters and at least two constructors (use constructor chaining)
Implement deposit() and withdraw() with proper validation
Add a display() method

Extend it with:

SavingsAccount (add interestRate, override display() and show interest)
CurrentAccount (add overdraftLimit, restrict withdrawals accordingly)

Your implementation should clearly show:

Proper encapsulation (no direct field access)
Use of constructor overloading and chaining (this(...))
Inheritance and method overriding (use @Override and super)
Polymorphism by storing objects in an Account reference list and calling display()
Basic validation/debugging (e.g., assert or exception for invalid operations)
