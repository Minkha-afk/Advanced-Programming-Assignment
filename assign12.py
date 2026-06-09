from abc import ABC, abstractmethod


class Order:
    def __init__(self, order_id, amount, customer_email, customer_phone):
        self.order_id = order_id
        self.amount = amount
        self.customer_email = customer_email
        self.customer_phone = customer_phone


class OrderType(ABC):
    @abstractmethod
    def calculate_final_amount(self, order):
        pass


class RegularOrder(OrderType):
    def calculate_final_amount(self, order):
        return order.amount


class DiscountedOrder(OrderType):
    def __init__(self, discount_percent):
        self.discount_percent = discount_percent

    def calculate_final_amount(self, order):
        discount = order.amount * (self.discount_percent / 100)
        return order.amount - discount


class PriorityOrder(OrderType):
    def __init__(self, priority_fee):
        self.priority_fee = priority_fee

    def calculate_final_amount(self, order):
        return order.amount + self.priority_fee


class PaymentMethod(ABC):
    @abstractmethod
    def pay(self, amount):
        pass


class CreditCardPayment(PaymentMethod):
    def pay(self, amount):
        print(f"Paid Rs. {amount} using Credit Card")
        return True


class UPIPayment(PaymentMethod):
    def pay(self, amount):
        print(f"Paid Rs. {amount} using UPI")
        return True


class WalletPayment(PaymentMethod):
    def pay(self, amount):
        print(f"Paid Rs. {amount} using Wallet")
        return True


class NotificationService(ABC):
    @abstractmethod
    def send_notification(self, order, message):
        pass


class EmailNotification(NotificationService):
    def send_notification(self, order, message):
        print(f"Email sent to {order.customer_email}: {message}")


class SMSNotification(NotificationService):
    def send_notification(self, order, message):
        print(f"SMS sent to {order.customer_phone}: {message}")


class PushNotification(NotificationService):
    def send_notification(self, order, message):
        print(f"Push notification sent: {message}")


class OrderStorage(ABC):
    @abstractmethod
    def save(self, order):
        pass


class DatabaseStorage(OrderStorage):
    def save(self, order):
        print(f"Order {order.order_id} saved in Database")


class FileStorage(OrderStorage):
    def save(self, order):
        print(f"Order {order.order_id} saved in File")


class OrderService:
    def __init__(self, payment_method, notification_service, order_storage, order_type):
        self.payment_method = payment_method
        self.notification_service = notification_service
        self.order_storage = order_storage
        self.order_type = order_type

    def place_order(self, order):
        final_amount = self.order_type.calculate_final_amount(order)

        payment_success = self.payment_method.pay(final_amount)

        if payment_success:
            self.order_storage.save(order)
            self.notification_service.send_notification(
                order,
                "Order placed successfully!"
            )
        else:
            self.notification_service.send_notification(
                order,
                "Payment failed!"
            )


def choose_payment_method():
    print("\nChoose Payment Method:")
    print("1. Credit Card")
    print("2. UPI")
    print("3. Wallet")

    choice = input("Enter choice: ")

    if choice == "1":
        return CreditCardPayment()
    elif choice == "2":
        return UPIPayment()
    elif choice == "3":
        return WalletPayment()
    else:
        print("Invalid choice. Default UPI selected.")
        return UPIPayment()


def choose_notification_service():
    print("\nChoose Notification Method:")
    print("1. Email")
    print("2. SMS")
    print("3. Push Notification")

    choice = input("Enter choice: ")

    if choice == "1":
        return EmailNotification()
    elif choice == "2":
        return SMSNotification()
    elif choice == "3":
        return PushNotification()
    else:
        print("Invalid choice. Default Email selected.")
        return EmailNotification()


def choose_storage():
    print("\nChoose Storage Method:")
    print("1. Database")
    print("2. File")

    choice = input("Enter choice: ")

    if choice == "1":
        return DatabaseStorage()
    elif choice == "2":
        return FileStorage()
    else:
        print("Invalid choice. Default Database selected.")
        return DatabaseStorage()


def choose_order_type():
    print("\nChoose Order Type:")
    print("1. Regular Order")
    print("2. Discounted Order")
    print("3. Priority Order")

    choice = input("Enter choice: ")

    if choice == "1":
        return RegularOrder()

    elif choice == "2":
        discount = float(input("Enter discount percentage: "))
        return DiscountedOrder(discount)

    elif choice == "3":
        fee = float(input("Enter priority delivery fee: "))
        return PriorityOrder(fee)

    else:
        print("Invalid choice. Default Regular Order selected.")
        return RegularOrder()


order_id = int(input("Enter Order ID: "))
amount = float(input("Enter Order Amount: "))
email = input("Enter Customer Email: ")
phone = input("Enter Customer Phone: ")

order = Order(order_id, amount, email, phone)

payment_method = choose_payment_method()
notification_service = choose_notification_service()
storage = choose_storage()
order_type = choose_order_type()

service = OrderService(
    payment_method,
    notification_service,
    storage,
    order_type
)

service.place_order(order)