class Address:
    def __init__(self, street: str, city: str, zip_code: str):
        self.street = street
        self.city = city
        self.zip_code = zip_code

    def __str__(self) -> str:
        """Clean string representation used inside Student.display()."""
        return f"{self.street}, {self.city} — {self.zip_code}"


class Student:

    def __init__(self, name: str, age: int, address: Address):
        self.name = name
        self.address = address
        self.age = age
        self._courses: list[str] = []

    @property
    def age(self) -> int:
        return self._age

    @age.setter
    def age(self, value: int) -> None:
        if not isinstance(value, int):
            raise TypeError(f"Age must be an integer, got {type(value).__name__}.")
        if value < 5 or value > 120:
            raise ValueError(f"Age must be between 5 and 120, got {value}.")
        self._age = value

    def add_course(self, course: str) -> None:
        """
        Adds a course to the student's list.
        Because lists are mutable, this change persists on the object —
        no reassignment needed.
        """
        if course in self._courses:
            print(f"  [!] '{course}' is already enrolled.")
            return
        self._courses.append(course)
        print(f"  [+] '{course}' added for {self.name}.")

    def display(self) -> None:
        courses = ", ".join(self._courses) if self._courses else "None"
        print("┌─── STUDENT ──────────────────────────────────")
        print(f"  Name    : {self.name}")
        print(f"  Age     : {self._age}")
        print(f"  Address : {self.address}")
        print(f"  Courses : {courses}")
        print("└──────────────────────────────────────────────")


class ScholarshipStudent(Student):

    def __init__(self, name: str, age: int, address: Address,
                 scholarship_amount: float):
        super().__init__(name, age, address)

        if scholarship_amount < 0:
            raise ValueError("Scholarship amount cannot be negative.")
        self.scholarship_amount = scholarship_amount

    def display(self) -> None:
        courses = ", ".join(self._courses) if self._courses else "None"
        print("┌─── SCHOLARSHIP STUDENT ──────────────────────")
        print(f"  Name         : {self.name}")
        print(f"  Age          : {self._age}")
        print(f"  Address      : {self.address}")
        print(f"  Courses      : {courses}")
        print(f"  Scholarship  : ₹{self.scholarship_amount:,.2f}")
        print("└──────────────────────────────────────────────")


def main():

    addr1 = Address("42 Nehru Street", "Tezpur", "784001")
    addr2 = Address("7 MG Road", "Guwahati", "781005")

    print("\n=== Creating student ===")
    s1 = Student("Minha", 0, addr1)

    print("\n=== Adding courses (mutable list) ===")
    s1.add_course("Data Structures")
    s1.add_course("Operating Systems")
    s1.add_course("Data Structures")

    print("\n=== Student info ===")
    s1.display()

    print("\n=== @property: update age ===")
    s1.age = 21
    print(f"  Updated age : {s1.age}")

    print("\n=== @property: validation catches bad values ===")
    try:
        s1.age = -5
    except ValueError as e:
        print(f"  ValueError  → {e}")

    try:
        s1.age = "twenty"
    except TypeError as e:
        print(f"  TypeError   → {e}")

    print("\n=== Creating ScholarshipStudent ===")
    s2 = ScholarshipStudent("Aryan", 22, addr2, scholarship_amount=75000)

    print("\n=== Adding courses to ScholarshipStudent ===")
    s2.add_course("Machine Learning")
    s2.add_course("Cloud Computing")

    print("\n=== ScholarshipStudent info ===")
    s2.display()

    print("\n=== Polymorphism: display() via common type ===")
    roster: list[Student] = [s1, s2]
    for student in roster:
        student.display()
        print()

    print("=== Mutable list: add course after display, re-display ===")
    s1.add_course("Computer Networks")
    print(f"  s1 courses now: {s1._courses}")


if __name__ == "__main__":
    main()
