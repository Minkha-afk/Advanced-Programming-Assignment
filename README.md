# Assignment 8

## Question

Develop a course enrollment dashboard in reactjs:

You are building a React component that displays enrolled students.

Each student:
{
  id: number,
  name: string,
  enrolledCourses: Set<string>,
  gpa: number
}

You must:

1. Maintain students in state.

2. Implement the following features:
a. Add new student
b. Remove student by ID
c. Display students sorted by GPA (descending)
d. Display all unique courses across students
e. Filter students enrolled in a specific course

3. Use the followings
a. Use useState
b. Use Map internally for id to student mapping
c. Use Set for course uniqueness
d. Use map, filter, and reduce
e. Do not mutate state directly
f. Use spread operator for updates
g. Convert Set to array before rendering

4. Compute time complexity of filtering students by course
