import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.io.TempDir;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

public class ScoreProcessorTest {
    @TempDir
    Path tempDir;

    @Test
    void validFileReturnsScoreTimesTen() throws IOException {
        Path file = tempDir.resolve("score.txt");
        Files.writeString(file, "8");

        ScoreProcessor processor = new ScoreProcessor();

        assertEquals(80, processor.processScoreFile(file.toString()));
    }

    @Test
    void missingFileThrowsException() {
        ScoreProcessor processor = new ScoreProcessor();

        assertThrows(RuntimeException.class,
                () -> processor.processScoreFile("missing.txt"));
    }

    @Test
    void invalidFileDataThrowsException() throws IOException {
        Path file = tempDir.resolve("bad-score.txt");
        Files.writeString(file, "abc");

        ScoreProcessor processor = new ScoreProcessor();

        assertThrows(NumberFormatException.class,
                () -> processor.processScoreFile(file.toString()));
    }
}
