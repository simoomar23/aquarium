import java.util.logging.*;
import java.io.IOException;

public class ClientLogger {
    private static final Logger logger = Logger.getLogger(ClientLogger.class.getName());

    static {
        try {
            FileHandler fh = new FileHandler("client.log", true);
            fh.setFormatter(new SimpleFormatter());

            logger.addHandler(fh);
            logger.setLevel(Level.ALL);              // Show all levels
            fh.setLevel(Level.ALL);                  // File handler accepts all levels
            logger.setUseParentHandlers(false);      // Don’t print to console
        } catch (IOException e) {
            System.err.println("Logger setup failed: " + e.getMessage());
        }
    }

    public static Logger getLogger() {
        return logger;
    }
}
