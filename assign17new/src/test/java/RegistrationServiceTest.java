import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertThrows;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class RegistrationServiceTest {
    private RegistrationService service;

    @BeforeEach
    void setUp() {
        service = new RegistrationService();
    }

    @Test
    void validUserCanRegister() throws InvalidEmailException {
        assertTrue(service.registerUser("student@example.com", 18));
    }

    @Test
    void invalidEmailThrowsInvalidEmailException() {
        assertThrows(InvalidEmailException.class,
                () -> service.registerUser("wrong-email", 20));
    }

    @Test
    void nullEmailThrowsInvalidEmailException() {
        assertThrows(InvalidEmailException.class,
                () -> service.registerUser(null, 20));
    }

    @Test
    void emptyEmailThrowsInvalidEmailException() {
        assertThrows(InvalidEmailException.class,
                () -> service.registerUser("", 20));
    }

    @Test
    void underageUserThrowsUnderageException() {
        assertThrows(UnderageException.class,
                () -> service.registerUser("minor@example.com", 17));
    }
}
