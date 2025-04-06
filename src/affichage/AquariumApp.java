import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import java.util.regex.*;
import java.util.ArrayList;
import java.util.Random;

public class AquariumApp extends Application {
    private ArrayList<Poisson> fishes = new ArrayList<>(); 

    private Image poissonImage1 = new Image("images/poisson1.png");
    private Image poissonImage2 = new Image("images/poisson2.png");
    private Image poissonImage3 = new Image("images/poisson3.png");

    private Image backgroundImage = new Image("images/aquarium_background.png");

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        Canvas canvas = new Canvas(500, 500);
        GraphicsContext gc = canvas.getGraphicsContext2D();

        Scene scene = new Scene(new StackPane(canvas), 500, 500);
        primaryStage.setTitle("Aquarium");
        primaryStage.setScene(scene);
        primaryStage.show();
	//	handleResponse("list [PoissonRouge at 90x4,10x4,1] [PoissonClown at 20x80,12x6,3]");	
        for (Poisson poisson : fishes) {
            setNewDestination(poisson);
        }

        AnimationTimer timer = new AnimationTimer() {
		private long lastUpdate = 0;

		@Override
		public void handle(long now) {
		    if (lastUpdate == 0) {
			lastUpdate = now;
			return;
		    }

		    double elapsedSeconds = (now - lastUpdate) / 1e9;
		    lastUpdate = now;

		    for (Poisson poisson : fishes) {
			poisson.updatePosition(elapsedSeconds);
			if (poisson.hasStarted() && poisson.hasReachedDestination()) {
			    setNewDestination(poisson);
			}
		    }

		    draw(gc);
		}
	    };
        timer.start();
    }

    private void addFish(String type, int x, int y, int width, int height, double remainingTime) {
        Poisson newFish = new Poisson((x * 500) / 100, (y * 500) /100, (width * 500) / 100, ( height * 500 ) / 100, type, remainingTime);
	newFish.setDestination((x * 500) / 100, (y * 500) / 100, remainingTime);
        fishes.add(newFish);
    }

    private void delFish(String type) {
        fishes.removeIf(poisson -> poisson.getType().equals(type));
    }

    private void startFish(String type) {
	fishes.stream().filter(poisson -> poisson.getType().equals(type)).findFirst().ifPresent(Poisson::startFish);
    }

    

    public void handleCommand(String command) {
	String[] parts = command.split(" ");
	if (parts.length >= 6 && parts[0].equals("addFish")) {
	    String type = parts[1];

	    String[] position = parts[3].replace(",", "").split("x");

	    String[] size = parts[4].replace(",", "").split("x");

	    int x = Integer.parseInt(position[0]);
	    int y = Integer.parseInt(position[1]);
	    System.out.println("x = " + x);
	    int width = Integer.parseInt(size[0]);
	    int height = Integer.parseInt(size[1]);
	    addFish(type, x, y, width, height, 0);
	
	} else if (parts.length >= 2 && parts[0].equals("delFish")) {
	    String type = parts[1];
	    delFish(type);
	} else if (parts.length == 2 && parts[0].equals("startFish")) {
	    String type = parts[1];
	    startFish(type);
	}   
    }

public void handleResponse(String response) {
    Pattern pattern = Pattern.compile("\\[([A-Za-z]+) at (\\d+)x(\\d+),(\\d+)x(\\d+),(\\d+)\\]");
    Matcher matcher = pattern.matcher(response);
    while (matcher.find()) {
        String type = matcher.group(1);
        int x = Integer.parseInt(matcher.group(2)); // destination X
        int y = Integer.parseInt(matcher.group(3)); // destination Y
        int width = Integer.parseInt(matcher.group(4));
        int height = Integer.parseInt(matcher.group(5));
        double time = Double.parseDouble(matcher.group(6));

        Poisson p = fishes.stream()
            .filter(poisson -> poisson.getType().equals(type))
            .findFirst()
            .orElse(null);

        if (p != null) {
            p.setDestination((x * 500) / 100, (y * 500) / 100, time);
        } else {
            addFish(type, x, y, width, height, time);
        }
    }
}
    
    
    private void setNewDestination(Poisson poisson) {
        // Random rand = new Random();
        // int x = rand.nextInt(100);
        // int y = rand.nextInt(100);
	// poisson.setDestination((x * 500) / 100, ( y * 500) / 100, 3.0);
	poisson.setDestination((poisson.getXdest() * 500) / 100, ( poisson.getYdest() * 500) / 100, poisson.getTime());	
    }

    private void draw(GraphicsContext gc) {
        gc.clearRect(0, 0, 500, 500);

        gc.drawImage(backgroundImage, 0, 0, 500, 500);

        for (Poisson poisson : fishes) {
            Image imageToDraw = getFishImage(poisson.getType());
            gc.drawImage(imageToDraw, poisson.getX(), poisson.getY(), poisson.getWidth(), poisson.getHeight());
        }
    }

    private Image getFishImage(String type) {
        switch (type) {
	case "Poisson1":
	    return poissonImage1;
	case "Poisson2":
	    return poissonImage2;
	case "Poisson3":
	    return poissonImage3;
	default:
	    return poissonImage1;
        }
    }
}
