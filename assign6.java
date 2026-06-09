import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

class Student {
    private int id;
    private String name;
    private List<String> courses;
    private Map<String, Integer> scores;

    public Student(int id, String name, List<String> courses, Map<String, Integer> scores) {
        this.id = id;
        this.name = name;
        this.courses = new ArrayList<>(courses);
        this.scores = new HashMap<>(scores);
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public List<String> getCourses() {
        return courses;
    }

    public Map<String, Integer> getScores() {
        return scores;
    }

    public double getAverageScore() {
        if (courses.size() == 0) {
            return 0;
        }

        return courses.stream()
                .mapToInt(course -> scores.getOrDefault(course, 0))
                .average()
                .orElse(0);
    }
}

public class assing {

    public static List<Student> getTopNStudents(List<Student> students, int n) {
        return students.stream()
                .sorted(Comparator.comparingDouble(Student::getAverageScore).reversed())
                .limit(n)
                .collect(Collectors.toCollection(ArrayList::new));
    }

    public static Map<String, Double> getAverageScorePerCourse(List<Student> students) {
        return students.stream()
                .flatMap(student -> student.getCourses().stream()
                        .map(course -> Map.entry(course, student.getScores().getOrDefault(course, 0))))
                .collect(Collectors.groupingBy(
                        Map.Entry::getKey,
                        HashMap::new,
                        Collectors.averagingInt(Map.Entry::getValue)
                ));
    }

    public static Set<String> getAllUniqueCourses(List<Student> students) {
        return students.stream()
                .flatMap(student -> student.getCourses().stream())
                .collect(Collectors.toCollection(HashSet::new));
    }

    public static void main(String[] args) {
        List<Student> students = new ArrayList<>();

        students.add(new Student(1, "Rahul",
                new ArrayList<>(Arrays.asList("Math", "Java", "DBMS")),
                new HashMap<>(Map.of("Math", 85, "Java", 90, "DBMS", 80))));

        students.add(new Student(2, "Priya",
                new ArrayList<>(Arrays.asList("Math", "Java", "OS")),
                new HashMap<>(Map.of("Math", 92, "Java", 88, "OS", 84))));

        students.add(new Student(3, "Arun",
                new ArrayList<>(Arrays.asList("Java", "DBMS", "OS")),
                new HashMap<>(Map.of("Java", 75, "DBMS", 81, "OS", 79))));

        students.add(new Student(4, "Sneha",
                new ArrayList<>(Arrays.asList("Math", "DBMS", "CN")),
                new HashMap<>(Map.of("Math", 89, "DBMS", 91, "CN", 87))));

        students.add(new Student(5, "Kiran",
                new ArrayList<>(Arrays.asList("Java", "CN", "OS")),
                new HashMap<>(Map.of("Java", 70, "CN", 73, "OS", 68))));

        System.out.println("Top 3 students based on average score:");
        List<Student> topStudents = getTopNStudents(students, 3);
        for (Student student : topStudents) {
            System.out.println(student.getName() + " - Average: " + student.getAverageScore());
        }

        System.out.println();
        System.out.println("Average score per course:");
        Map<String, Double> avgMap = getAverageScorePerCourse(students);
        for (Map.Entry<String, Double> entry : avgMap.entrySet()) {
            System.out.println(entry.getKey() + " : " + entry.getValue());
        }

        System.out.println();
        System.out.println("All unique courses:");
        Set<String> courses = getAllUniqueCourses(students);
        for (String course : courses) {
            System.out.println(course);
        }

        System.out.println();
        System.out.println("Complexity Analysis:");
        System.out.println("1. Time complexity of computing course averages is O(S * C)");
        System.out.println("   because we go through each student's course list once.");
        System.out.println("2. Complexity of sorting top N students is O(S * C + S log S)");
        System.out.println("   because average is computed and students are sorted.");
    }
}
