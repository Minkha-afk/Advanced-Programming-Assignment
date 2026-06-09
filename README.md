# Assignment 12

## Question

Design a system in java/python for processing customer orders in an e-commerce platform.

An order system should support:

Multiple payment methods (Credit Card, UPI, Wallet, etc.)
Multiple notification channels (Email, SMS, Push)
Different order types (Regular Order, Discounted Order, Priority Order)
Ability to store order data using different storage mechanisms (Database, File, etc.)
Design Constraints (Must Apply SOLID Principles)

Your design must satisfy the SOLID principles as follows:

1. Single Responsibility Principle (SRP): Each class should have a single responsibility
(e.g., order logic, payment processing, notification, storage should be separate)

2. Open/Closed Principle (OCP): You should be able to add:
New payment methods
New notification types
Without modifying existing classes

3. Liskov Substitution Principle (LSP): All subclasses (e.g., payment types, order types) should work correctly when used through their base type, No subclass should break expected behavior

4. Interface Segregation Principle (ISP): Avoid large interfaces, Design small, role-specific interfaces
(e.g., don't force all classes to implement unused methods)

5. Dependency Inversion Principle (DIP): High-level classes (e.g., OrderService) must depend on abstractions, not concrete implementations, Use dependency injection.

Your system should:

Create an order,
Process payment using a selected payment method,
Send notification after successful order,
Save order details using a storage mechanism;
