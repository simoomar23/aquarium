import java.io.*;
import java.net.*;
import java.util.Scanner;
import javafx.application.Application;
import javafx.stage.Stage;

public class Client extends Application {
    static final int port = 9090;
    private PrintWriter pred;
    private BufferedReader plec;
    private AquariumApp app;
    private static String id;

    public static void main(String[] args) {
        id = args[1];
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        app = new AquariumApp();
        app.start(primaryStage);

        new Thread(() -> {
            try {
                startClient("localhost");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();
    }

public void startClient(String serverAddress) throws IOException {
    Socket socket = new Socket(serverAddress, port);
    plec = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    pred = new PrintWriter(socket.getOutputStream(), true);

    pred.println("hello in as " + id);
    String greeting = plec.readLine();
    System.out.println(greeting);
    
    new Thread(() -> {
        try {
            String serverResponse;
            while ((serverResponse = plec.readLine()) != null) {
		System.out.println("   > " + serverResponse);
                
                if (serverResponse.startsWith("list")) {
                    handleListResponse(serverResponse);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }).start();

    Scanner scanner = new Scanner(System.in);
    while (true) {
	//        System.out.print("$ ");
        String command = scanner.nextLine();
        if (command.equalsIgnoreCase("log out")) break;
   
        pred.println(command);
        javafx.application.Platform.runLater(() -> app.handleCommand(command));
    }
    socket.close();
}

private void handleListResponse(String response) {
    javafx.application.Platform.runLater(() -> app.handleResponse(response));
}
}
