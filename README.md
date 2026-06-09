# Assignment 18

## Question

Generally, handling external files is a common source of runtime errors. Files might be missing, or they might contain corrupted data. For this assignment, you will create a simple data utility class that reads an integer value from a text file, performs a calculation, and ensures all system resources are properly closed afterward-even if something goes wrong.

Your program needs to read a numeric score from a file, multiply it by 10, and return the result. If the file does not exist, the system must catch that error and notify the user with a specific message. If the file exists but contains letters instead of a number, the system must handle that invalid data format gracefully. Finally, you must write basic automated tests to verify that your calculation works and that bad inputs are handled correctly.

Implementation Rules:

If choosing Java: Create a class named ScoreProcessor. Write a method public int processScoreFile(String filePath) that uses a try-catch-finally block (or a try-with-resources block) to open and read a file. Catch FileNotFoundException and NumberFormatException specifically, logging or printing a clear error message for each. Use the finally block to print a "File cleanup completed" message to the console. Write a JUnit 5 test suite with at least two test cases: one verifying a successful calculation with a valid file path, and one using assertThrows to check how the system reacts to a missing file.

If choosing Python: Create a class named ScoreProcessor. Write a method def process_score_file(self, file_path: str) -> int that uses a try-except-else-finally block to open and read a file. Catch FileNotFoundError and ValueError specifically, printing a helpful error message for each. Use the else block to print "Data processed successfully", and use the finally block to print "File cleanup completed". Write a pytest suite with at least two test functions: one testing a successful calculation with a valid file, and one using with pytest.raises to verify that a missing file is handled correctly.

You must have the followings:

Exception Handling & Structure: Correctly implementing the multi-catch structure (FileNotFound and Invalid Format/Value errors) and ensuring the cleanup block executes under all conditions.

Core Logic & Input Validation: Successfully reading the file content, parsing the text into a usable integer, and executing the required multiplication calculation.

Unit Testing: Setting up a working test suite that uses correct framework assertions to validate both the happy path (successful calculation) and the error path (missing file).
