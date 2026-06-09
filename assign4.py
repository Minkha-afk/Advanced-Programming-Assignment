# List of products
products = [
    {"name": "Laptop", "stock": 15},
    {"name": "Mouse", "stock": 8},
    {"name": "Keyboard", "stock": 5},
    {"name": "Monitor", "stock": 12},
    {"name": "Printer", "stock": 3}
]

# Display products with stock less than 10
print("Products with stock less than 10:")
for product in products:
    if product["stock"] < 10:
        print(f"{product['name']} - Stock: {product['stock']}")