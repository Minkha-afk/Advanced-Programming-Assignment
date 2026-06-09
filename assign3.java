import java.util.ArrayList;
import java.util.Scanner;

public class BookSearch {
    public static void main(String[] args) {
        // Create an ArrayList to store book titles
        ArrayList<String> books = new ArrayList<>();

        // Add books to the list
        books.add("The Great Gatsby");
        books.add("Java Programming Basics");
        books.add("Data Structures and Algorithms");
        books.add("Introduction to Computer Science");
        books.add("Advanced Java Concepts");

        // Input search word
        Scanner sc = new Scanner(System.in);
        System.out.print("Enter a word to search in book titles: ");
        String keyword = sc.nextLine();

        // Search for matching book titles
        System.out.println("\nBooks containing \"" + keyword + "\":");
        boolean found = false;

        for (String book : books) {
            if (book.toLowerCase().contains(keyword.toLowerCase())) {
                System.out.println(book);
                found = true;
            }
        }

        if (!found) {
            System.out.println("No matching books found.");
        }

        sc.close();
    }
}