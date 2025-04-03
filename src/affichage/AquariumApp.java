import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

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

        addFish("Poisson1", 50, 50, 100, 100);
        addFish("Poisson2", 100, 100, 100, 100);
        addFish("Poisson3", 200, 200, 100, 100);

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
                    if (poisson.hasReachedDestination()) {
                        setNewDestination(poisson);
                    }
                }

                draw(gc);
            }
        };
        timer.start();
    }

    private void addFish(String type, int x, int y, int width, int height) {
        Poisson newFish = new Poisson(x, y, width, height, type);
        fishes.add(newFish);
    }

    private void delFish(String type) {
        fishes.removeIf(poisson -> poisson.getType().equals(type));
    }

    private void setNewDestination(Poisson poisson) {
        Random rand = new Random();
        int x = rand.nextInt(400);
        int y = rand.nextInt(400);
        poisson.setDestination(x, y, 3.0);
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
