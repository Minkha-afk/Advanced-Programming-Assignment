public class Main {
    public static void main(String[] args) {
        RegistrationService service = new RegistrationService();

        try {
            boolean result = service.registerUser("student@example.com", 20);
            System.out.println("Registration successful: " + result);
        } catch (InvalidEmailException e) {
            System.out.println(e.getMessage());
        } catch (UnderageException e) {
            System.out.println(e.getMessage());
        }
    }
}

class RegistrationService {
    private static final String EMAIL_REGEX = "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$";

    public boolean registerUser(String email, int age) throws InvalidEmailException {
        assert EMAIL_REGEX != null : "Email regex must exist";

        if (email == null || email.trim().isEmpty()) {
            throw new InvalidEmailException("Invalid email: " + email);
        }

        if (!email.matches(EMAIL_REGEX)) {
            throw new InvalidEmailException("Invalid email: " + email);
        }

        if (age < 18) {
            throw new UnderageException("Age " + age + " is under 18");
        }

        return true;
    }
}

class InvalidEmailException extends Exception {
    public InvalidEmailException(String message) {
        super(message);
    }
}

class UnderageException extends RuntimeException {
    public UnderageException(String message) {
        super(message);
    }
}
