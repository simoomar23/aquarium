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

public class Client extends Application {
    static final int port = 9091;
    private PrintWriter pred;
    private BufferedReader plec;
    private AquariumApp app;
    private static String id;
    private static String adress;
    BlockingQueue<String> okQueue = new LinkedBlockingQueue<>();
    TextArea receiveArea = new TextArea();
    TextField inputField = new TextField();

    public static void main(String[] args) {
        id = args[1];
        adress = args[0];
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        app = new AquariumApp();
        setupSendWindow();
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
	int start = Math.max(0, lines.length - 19); // 19, car on va ajouter 1 ligne
	for (int i = start; i < lines.length; i++) {
	    sb.append(lines[i]).append("\n");
	}
	sb.append(text).append("\n");
	receiveArea.setText(sb.toString());
    }

    public int startClient(Stage primaryStage) throws IOException {
        Socket socket = new Socket(adress, port);
        plec = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        pred = new PrintWriter(socket.getOutputStream(), true);
        String connexion = "hello in as " + id;
        pred.println(connexion);
        String greeting = plec.readLine();

        Platform.runLater(() -> {
            app.handleCommand(connexion);
            app.handleCommand(greeting);
            appendLimitedText(" > " + greeting + "\n");
        });

        pred.println("getFishesContinuously");

        new Thread(() -> {
            try {
                String serverResponse;
                while ((serverResponse = plec.readLine()) != null) {
                    final String msg = serverResponse;
                    Platform.runLater(() -> appendLimitedText(" > " + msg + "\n"));

                    if (msg.startsWith("list")) {
                        handleListResponse(msg);
                    } else if (msg.startsWith("OK")) {
                        okQueue.put("OK");
                    } else if (msg.startsWith("bye")) {
                        pred.close();
                        plec.close();
                        socket.close();
                        Platform.runLater(() -> primaryStage.close());
                        System.exit(0);
                    }
                }
            } catch (IOException | InterruptedException e) {
                e.printStackTrace();
            }
        }).start();

        return 0;
    }

    private void handleListResponse(String response) {
        Platform.runLater(() -> app.handleResponse(response));
    }

    private void setupSendWindow() {
        Stage sendStage = new Stage();
        VBox layout = new VBox();
        Button sendBtn = new Button("Send");

        sendBtn.setOnAction(e -> {
            String command = inputField.getText();
            if (command == null || command.isBlank()) return;

            if (command.equalsIgnoreCase("log out")) {
                try {
                    pred.println("bye");
                    Platform.exit();
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
            } else if (command.startsWith("status")) {
                app.handleCommand(command);
            } else {
                pred.println(command);
                if (command.startsWith("addFish") || command.startsWith("delFish") || command.startsWith("startFish")) {
                    new Thread(() -> {
                        try {
                            String ok = okQueue.take();
                            if (ok.equals("OK")) {
                                Platform.runLater(() -> app.handleCommand(command));
                            }
                        } catch (InterruptedException ex) {
                            ex.printStackTrace();
                        }
                    }).start();
                }
            }
            inputField.clear();
        });

        layout.getChildren().addAll(new Label("Enter Command:"), inputField, sendBtn);
        sendStage.setScene(new Scene(layout, 300, 100));
        sendStage.setTitle("Send Commands");
        sendStage.show();
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
