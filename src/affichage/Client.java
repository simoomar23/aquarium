import java.io.*;
import java.net.*;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
<<<<<<< HEAD
import javafx.application.Application;
import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public class Client extends Application {
    static final int port = 9091;
=======
import java.util.logging.Logger;


public class Client extends Application {
    //static final int port = 9092;
    private static int port;
>>>>>>> refs/remotes/origin/main
    private PrintWriter pred;
    private BufferedReader plec;
    private AquariumApp app;
    private static String id;
    private static String adress;
    BlockingQueue<String> okQueue = new LinkedBlockingQueue<>();
<<<<<<< HEAD
    TextArea receiveArea = new TextArea();
    TextField inputField = new TextField();

    public static void main(String[] args) {
        id = args[1];
        adress = args[0];
        launch(args);
=======
    private static final Logger logger = ClientLogger.getLogger();

//    public static void main(String[] args) {
//        id = args[1];
//	adress = args[0];
//        launch(args);
//    }
    public static void main(String[] args) {
    try {
        DisplayConfig config = new DisplayConfig("affichage.cfg");

        adress = config.get("controller-address");
        port = config.getInt("controller-port", 9092); //9092 default value hmmm??
        id = config.get("id");

        logger.info("Loaded config: address=" + adress + ", port=" + port + ", id=" + id);
    } catch (IOException e) {
        logger.severe("Failed to load config: " + e.getMessage());
        System.err.println("Could not load configuration: " + e.getMessage());
        System.exit(1);
>>>>>>> refs/remotes/origin/main
    }

    launch(args);  // JavaFX starts here
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

<<<<<<< HEAD
    public int startClient(Stage primaryStage) throws IOException {
        Socket socket = new Socket(adress, port);
        plec = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        pred = new PrintWriter(socket.getOutputStream(), true);
        String connexion = "hello in as " + id;
        pred.println(connexion);
        String greeting = plec.readLine();
=======
public int startClient(Stage primaryStage) throws IOException {
    
    
    Socket socket = new Socket(adress, port);
    logger.info("Connected to server at " + adress + ":" + port);
    plec = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    pred = new PrintWriter(socket.getOutputStream(), true);
    String connexion = "hello in as " + id;
    pred.println(connexion);
    logger.info("Hello in as: " + id);
    String greeting = plec.readLine();
    System.out.println("    > " + greeting);
    logger.info("Server greeting: " + greeting);

    javafx.application.Platform.runLater(() -> app.handleCommand(connexion));
    pred.println("getFishesContinuously");
    
    new Thread(() -> {
        try {
            String serverResponse;
            while ((serverResponse = plec.readLine()) != null) {
                System.out.println("   > " + serverResponse);
                logger.fine("Received from server: " + serverResponse);
                
                if (serverResponse.startsWith("list")) {
                    logger.info("Handling list response.");
                    handleListResponse(serverResponse);
                } else if (serverResponse.startsWith("OK")) {
                    logger.info("Received OK from server.");
                    try {
                        okQueue.put("OK");
                    } catch (InterruptedException e) {
                        logger.warning("Interrupted while putting OK in queue: " + e.getMessage());
                        e.printStackTrace();
                    }
                } else if (serverResponse.startsWith("bye")) {
                    // Fermer la connexion
                    pred.close();
                    plec.close();
                    socket.close();
                    System.out.println("Disconnected from server.");
                    logger.info("Received 'bye' from server. Closing connection.");
>>>>>>> refs/remotes/origin/main

        Platform.runLater(() -> {
            app.handleCommand(connexion);
            app.handleCommand(greeting);
            receiveArea.appendText(" > " + greeting + "\n");
        });

        pred.println("getFishesContinuously");

        new Thread(() -> {
            try {
                String serverResponse;
                while ((serverResponse = plec.readLine()) != null) {
                    final String msg = serverResponse;
                    Platform.runLater(() -> receiveArea.appendText(" > " + msg + "\n"));

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
<<<<<<< HEAD
            inputField.clear();
        });

        layout.getChildren().addAll(new Label("Enter Command:"), inputField, sendBtn);
        sendStage.setScene(new Scene(layout, 300, 100));
        sendStage.setTitle("Send Commands");
        sendStage.show();
    }
=======
        } catch (IOException e) {
            logger.severe("IOException in thread reading server responses: " + e.getMessage());
            e.printStackTrace();
        }
    }).start();
    
    // Reste de ton code pour la gestion des commandes du client

    Scanner scanner = new Scanner(System.in);
    while (true) {
	//        System.out.print("$ ");
        String command= scanner.nextLine();
        logger.fine("User input command: " + command);
        if (command.equalsIgnoreCase("log out")) {
             logger.info("User issued 'log out'. Exiting.");
            break;
        }
	else if (socket.isClosed()) {
	    System.out.println("-> NOK : Non Connecté au contrôleur !");
        logger.warning("Attempted to send command while socket is closed: " + command);
	}
	else if (command.startsWith("status")) {
        logger.info("Executing 'status' command via JavaFX.");
	    javafx.application.Platform.runLater(() -> app.handleCommand(command));		
    }	    
	else {	    
        pred.println(command);
        logger.info("Sent command to server: " + command);
    if (command.startsWith("addFish") || command.startsWith("delFish") || command.startsWith("startFish")) {
	String ok = null;
	try {
	    ok = okQueue.take();
	} catch (InterruptedException e) {
	    e.printStackTrace();
	}
        if (ok.equals("OK")) {
	    //	    System.out.println(command);
            logger.info("OK received for command. Running in GUI: " + command);
            javafx.application.Platform.runLater(() -> app.handleCommand(command));
        }
    }
	}
    }
    socket.close();
    logger.info("Socket closed after command loop.");
    return 0;
}
>>>>>>> refs/remotes/origin/main

    private void setupReceiveWindow() {
        Stage receiveStage = new Stage();
        receiveArea.setEditable(false);
        VBox layout = new VBox(receiveArea);
        receiveStage.setScene(new Scene(layout, 400, 300));
        receiveStage.setTitle("Server Messages");
        receiveStage.show();
    }
}
