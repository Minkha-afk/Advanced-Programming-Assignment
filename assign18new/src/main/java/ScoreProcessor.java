import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class ScoreProcessor {
    public int processScoreFile(String filePath) {
        Scanner scanner = null;

        try {
            scanner = new Scanner(new File(filePath));
            String text = scanner.nextLine();
            int score = Integer.parseInt(text.trim());
            return score * 10;
        } catch (FileNotFoundException e) {
            System.out.println("Error: File not found");
            throw new RuntimeException("File not found");
        } catch (NumberFormatException e) {
            System.out.println("Error: File contains invalid data");
            throw e;
        } finally {
            if (scanner != null) {
                scanner.close();
            }
            System.out.println("File cleanup completed");
        }
    }
}
