# Assignment 10

## Question

Design a student system in Python with:

Address class (street, city, zipCode)
Student class with name, age, Address, and course list
Store age as a protected attribute and control it using @property
Methods: add_course() and display()

Extend it with:

ScholarshipStudent (add scholarshipAmount and override display())

Your implementation should clearly show:

Composition (Student HAS-A Address)
Proper data validation using @property (age must be valid)
Inheritance and overriding (use super() in display)
Understanding of mutable behavior (course list updates persist)
