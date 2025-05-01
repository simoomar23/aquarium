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
    private int VUE_WIDTH = 500;    
    private int VUE_HEIGHT = 500;
    private String ID;
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
        Canvas canvas = new Canvas(VUE_WIDTH, VUE_HEIGHT);
        GraphicsContext gc = canvas.getGraphicsContext2D();

        Scene scene = new Scene(new StackPane(canvas), VUE_WIDTH, VUE_HEIGHT);
        primaryStage.setTitle("Aquarium");
        primaryStage.setScene(scene);
        primaryStage.show();
        for (Poisson poisson : fishes) {
	    poisson.setDestination(poisson.getXdest(), poisson.getYdest(), poisson.getTime());			    
	    //            setNewDestination(poisson);
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
			//			System.out.println("x : " + (poisson.getX() * 500) / 100 +  ", x_dest : " + (poisson.getXdest() * 500) / 100);
			if (!poisson.hasReachedDestination()) {
			    //			    System.out.println("Test");
			    poisson.setDestination(poisson.getXdest(), poisson.getYdest(), poisson.getTime());					    
			    //			    setNewDestination(poisson);
			}
		    }

		    draw(gc);
		}
	    };
        timer.start();
    }

    private void addFish(String type, int x, int y, int width, int height, double remainingTime) {
        Poisson newFish = new Poisson((x * VUE_WIDTH) / 100, (y * VUE_HEIGHT) /100, (width * VUE_WIDTH) / 100, ( height * VUE_HEIGHT ) / 100, type, remainingTime);
	newFish.setDestination((x * VUE_WIDTH) / 100, (y * VUE_HEIGHT) / 100, remainingTime);
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
	if (parts.length == 6 && parts[0].equals("addFish")) {
	    String type = parts[1];

	    String[] position = parts[3].replace(",", "").split("x");

	    String[] size = parts[4].replace(",", "").split("x");
	    
	    int x = Integer.parseInt(position[0]);
	    int y = Integer.parseInt(position[1]);
	    int width = Integer.parseInt(size[0]);
	    int height = Integer.parseInt(size[1]);
	    if (width < 0 || height < 0 || ((x < 0 && x + width < 0 ) && (y < 0 && y + height < 0))){
		System.out.println("   -> NOK : commande introuvable");
		return;
	    }
	    addFish(type, x, y, width, height, 0);
	}
	    else if (parts.length == 2 && parts[0].equals("delFish")) {
	    String type = parts[1];
	    delFish(type);
	}  else if (parts.length == 2 && parts[0].equals("startFish")) {
	    String type = parts[1];
	    startFish(type);
	} else if (parts.length == 1 && parts[0].equals("status")) {
		System.out.println("    -> OK : Connecté au contrôleur, " + fishes.size()+ "poissons trouvés");
		for (Poisson poisson : fishes) {
		    if (poisson.hasStarted())
			System.out.println("Fish " + poisson.getType() + " at " + (poisson.getX() * 100) / 500 + "x" + (poisson.getY() * 100) /500 + "," + (poisson.getWidth() * 100 ) / 500 + "x" + (poisson.getHeight() * 100 ) / 500 + " started");
		    else
			System.out.println("Fish " + poisson.getType() + " at " + (poisson.getX() * 100) / 500 + "x" + (poisson.getY() * 100) /500 + "," + (poisson.getWidth() * 100 ) / 500 + "x" + (poisson.getHeight() * 100 ) / 500 + " notstarted");		  
		}
	    } else if (parts.length == 4 && parts[0].equals("hello")) {
		ID = parts[3];
	    } else if (parts.length == 3 && parts[0].equals("greeting")) {
		Pattern p = Pattern.compile("\\+([0-9]+)\\+([0-9]+)$");
		Matcher m = p.matcher(parts[2]);
		if (m.find()) {
		    VUE_WIDTH = Integer.parseInt(m.group(1));
		    VUE_HEIGHT = Integer.parseInt(m.group(2));
		}
	    } else {
		System.out.println("   -> NOK : commande introuvable");
	    }
    }


    public void handleResponse(String response) {
	//	Pattern pattern = Pattern.compile("\\[([A-Za-z]+) at (\\d+)x(\\d+),(\\d+)x(\\d+),(\\d+)\\]");
	Pattern pattern = Pattern.compile(".*\\[([A-Za-z0-9]+) at (\\d+)x(\\d+),(\\d+)x(\\d+),(\\d+)\\]");
	Matcher matcher = pattern.matcher(response);
	//	System.out.println("cccccccccccccc");	
	while (matcher.find()) {
	    //	    System.out.println("ddddddddddddd");		    
	    String type = matcher.group(1);
	    int x = Integer.parseInt(matcher.group(2));
	    int y = Integer.parseInt(matcher.group(3));
	    int width = Integer.parseInt(matcher.group(4));
	    int height = Integer.parseInt(matcher.group(5));
	    double time = Double.parseDouble(matcher.group(6));

	    Poisson p = fishes.stream()
		.filter(poisson -> poisson.getType().equals(type))
		.findFirst()
		.orElse(null);
	    if (p != null) {
		//		System.out.println("x : " + x + " y : " + y);
		p.setDestination((x * VUE_WIDTH) / 100, (y * VUE_HEIGHT) / 100, time);
		//		System.out.println("x : " + p.getX() + " y : " + p.getY() + "x_dest : " + p.getXdest() + " y_dest : " + p.getYdest());
	    } else {
		//		System.out.println("bbbbbbbbbbbbbb");		
		addFish(type, x, y, width, height, time);
	    }
	}
    }
    
    
    private void setNewDestination(Poisson poisson) {
        // Random rand = new Random();
        // int x = rand.nextInt(100);
        // int y = rand.nextInt(100);
	// poisson.setDestination((x * VUE_WIDTH) / 100, ( y * VUE_HEIGHT) / 100, 3.0);
	//	poisson.setDestination((poisson.getXdest() * VUE_WIDTH) / 100, ( poisson.getYdest() * VUE_HEIGHT) / 100, poisson.getTime());
	poisson.setDestination(poisson.getXdest(), poisson.getYdest(), poisson.getTime());		
    }

    private void draw(GraphicsContext gc) {
        gc.clearRect(0, 0, VUE_WIDTH, VUE_HEIGHT);

        gc.drawImage(backgroundImage, 0, 0, VUE_WIDTH, VUE_HEIGHT);

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
