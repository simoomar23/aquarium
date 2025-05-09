import java.io.*;
import java.net.*;
import java.util.Scanner;
import javafx.application.Application;
import javafx.stage.Stage;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Client extends Application {
    static final int port = 9091;
    private PrintWriter pred;
    private BufferedReader plec;
    private AquariumApp app;
    private static String id;
    private static String adress;
    BlockingQueue<String> okQueue = new LinkedBlockingQueue<>();
    
    public static void main(String[] args) {
        id = args[1];
	adress = args[0];
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

public int startClient(String serverAddress
		       ) throws IOException {
    Socket socket = new Socket(adress, port);
    plec = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    pred = new PrintWriter(socket.getOutputStream(), true);
    String connexion = "hello in as N" + id;
    pred.println(connexion);
    String greeting = plec.readLine();
    System.out.println("    > " + greeting);
    javafx.application.Platform.runLater(() -> app.handleCommand(connexion));
    new Thread(() -> {
        try {
            String serverResponse;
            while ((serverResponse = plec.readLine()) != null) {
		System.out.println("   > " + serverResponse);
                
                if (serverResponse.startsWith("list")) {
		    //		    System.out.println("sssssssssssss");
                    handleListResponse(serverResponse);
                } else if (serverResponse.startsWith("OK")) {
		    try {
			okQueue.put("OK");
		    } catch (InterruptedException e) {
			e.printStackTrace();
		    }		   
		}   else if (serverResponse.startsWith("bye")) {
		    break;
            }
	    } }catch (IOException e) {
            e.printStackTrace();
        }
    }).start();

    Scanner scanner = new Scanner(System.in);
    while (true) {
	//        System.out.print("$ ");
        String command= scanner.nextLine();
        if (command.equalsIgnoreCase("log out")) break;
	else if (socket.isClosed()) {
	    System.out.println("-> NOK : Non Connecté au contrôleur !");
	}
	else if (command.startsWith("status"))
	    javafx.application.Platform.runLater(() -> app.handleCommand(command));			    
	else {	    
        pred.println(command);
    if (command.startsWith("addFish") || command.startsWith("delFish") || command.startsWith("startFish")) {
	String ok = null;
	try {
	    ok = okQueue.take();
	} catch (InterruptedException e) {
	    e.printStackTrace();
	}
        if (ok.equals("OK")) {
	    //	    System.out.println(command);
            javafx.application.Platform.runLater(() -> app.handleCommand(command));
        }
    }
	}
    }
    socket.close();
    return 0;
}

private void handleListResponse(String response) {
    javafx.application.Platform.runLater(() -> app.handleResponse(response));
}
}
