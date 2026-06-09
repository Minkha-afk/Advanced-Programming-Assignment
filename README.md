# Assignment 6

## Question

Develop a student performance analyzer in Java. You are given a list of students of your batch. Each student has:
id (int) //don't include CSB string
name (String)
courses (List<String>)
scores (Map<String, Integer>) where key = course, value = marks

Do:
1. Store students using appropriate collections.
2. Implement the following methods:
List<Student> getTopNStudents(List<Student> students, int n);
Map<String, Double> getAverageScorePerCourse(List<Student> students);
Set<String> getAllUniqueCourses(List<Student> students);

Must use:

1. Use ArrayList, HashMap, and HashSet
2. Use Streams for aggregation and filtering
3. Sort students by average score (descending)
4. Use Comparator
5. Handle missing course scores using getOrDefault
6. Ensure type safety using generics

Perform complexity analysis:
1. What is the time complexity of computing course averages?
2. What is the complexity of sorting top N students?
