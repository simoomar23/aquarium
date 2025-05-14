import java.io.*;
import java.net.*;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import java.util.logging.Logger;

public class Client extends Application {
    private static int port;
    private PrintWriter pred;
    private BufferedReader plec;
    private AquariumApp app;
    private static String id;
    private static String adress;
    BlockingQueue<String> okQueue = new LinkedBlockingQueue<>();
    TextArea receiveArea = new TextArea();

    private static final Logger logger = ClientLogger.getLogger();

    public static void main(String[] args) {
        try {
            DisplayConfig config = new DisplayConfig("affichage.cfg");
            adress = config.get("controller-address");
            port = config.getInt("controller-port", 9092);
            id = config.get("id");
            logger.info("Loaded config: address=" + adress + ", port=" + port + ", id=" + id);
        } catch (IOException e) {
            logger.severe("Failed to load config: " + e.getMessage());
            System.err.println("Could not load configuration: " + e.getMessage());
            System.exit(1);
        }

        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        app = new AquariumApp();
        setupReceiveWindow();

        new Thread(() -> {
            try {
                startClient(primaryStage);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();
    }

    private void appendLimitedText(String text) {
        String[] lines = receiveArea.getText().split("\n");
        StringBuilder sb = new StringBuilder();
        int start = Math.max(0, lines.length - 19);
        for (int i = start; i < lines.length; i++) {
            sb.append(lines[i]).append("\n");
        }
        sb.append(text).append("\n");
        receiveArea.setText(sb.toString());
    }

    public int startClient(Stage primaryStage) throws IOException {
        Socket socket = new Socket(adress, port);
        logger.info("Connecting to server at " + adress + ":" + port);
        plec = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        pred = new PrintWriter(socket.getOutputStream(), true);
        String connexion = "hello in as " + id;

        pred.println(connexion);
        logger.fine("Sending connection message: " + connexion);
        String greeting = plec.readLine();
        logger.fine("Received greeting: " + greeting);

        Platform.runLater(() -> {
            app.handleCommand(connexion);
            app.handleCommand(greeting);
            appendLimitedText(" > " + greeting + "\n");
        });

        pred.println("getFishesContinuously");
        logger.fine("Sending to server getFishedContinuously");

        new Thread(() -> {
            try {
                String serverResponse;
                while ((serverResponse = plec.readLine()) != null) {
                    final String msg = serverResponse;
                    logger.fine("Received message from server: " + msg);

                    Platform.runLater(() -> appendLimitedText(" > " + msg + "\n"));

                    if (msg.startsWith("list")) {
                        handleListResponse(msg);
                    } else if (msg.startsWith("OK")) {
                        okQueue.put("OK");
                    } else if (msg.startsWith("bye")) {
                        logger.info("Server requested disconnect.");
                        pred.close();
                        plec.close();
                        socket.close();
                        Platform.runLater(() -> primaryStage.close());
                        System.exit(0);
                    }
                }
            } catch (IOException | InterruptedException e) {
                logger.log(java.util.logging.Level.SEVERE, "Error during communication", e);
                e.printStackTrace();
            }
        }).start();

        new Thread(() -> {
            BufferedReader consoleInput = new BufferedReader(new InputStreamReader(System.in));
            try {
                while (true) {
                    String command = consoleInput.readLine();
                    logger.fine("User entered command: " + command);

                    if (command == null || command.isBlank()) continue;

                    if (command.equalsIgnoreCase("log out")) {
                        logger.fine("Sent log out to server");
                        logger.fine("Logging out. Platform closing...");
                        pred.println("bye");
                        Platform.exit();
                        break;
                    } else if (command.startsWith("status")) {
                        app.handleCommand(command);
                    } else {
                        logger.fine("Sent command to server:" + command);
                        pred.println(command);
                        if (command.startsWith("addFish") || command.startsWith("delFish") || command.startsWith("startFish")) {
                            String ok = okQueue.take();
                            if (ok.equals("OK")) {
                                Platform.runLater(() -> app.handleCommand(command));
                            }
                        }
                    }
                }
            } catch (IOException | InterruptedException ex) {
                ex.printStackTrace();
            }
        }).start();

        return 0;
    }

    private void handleListResponse(String response) {
        Platform.runLater(() -> app.handleResponse(response));
    }

    private void setupReceiveWindow() {
        Stage receiveStage = new Stage();
        receiveArea.setEditable(false);
        VBox layout = new VBox(receiveArea);
        receiveStage.setScene(new Scene(layout, 400, 300));
        receiveStage.setTitle("Server Messages");
        receiveStage.show();
    }
}
