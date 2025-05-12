import java.io.*;
import java.util.*;

public class DisplayConfig {
    private final Properties properties = new Properties();

    public DisplayConfig(String filepath) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filepath))) {
            properties.load(new java.io.StringReader(removeComments(reader)));
        }
    }

    private String removeComments(BufferedReader reader) throws IOException {
        StringBuilder cleaned = new StringBuilder();
        String line;
        while ((line = reader.readLine()) != null) {
            line = line.trim();
            if (!line.startsWith("#") && !line.isEmpty()) {
                cleaned.append(line).append(System.lineSeparator());
            }
        }
        return cleaned.toString();
    }

    public String get(String key) {
        return properties.getProperty(key);
    }

    public int getInt(String key, int defaultValue) {
        try {
            return Integer.parseInt(get(key));
        } catch (NumberFormatException | NullPointerException e) {
            return defaultValue;
        }
    }
}
