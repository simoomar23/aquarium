import javafx.scene.image.Image;
class Poisson {
    private double x, y, x_destination, y_destination;
    private int width, height;
    private double remainingTime;
    private String type;
    private boolean isStarted;
    private double startX, startY;
    private double totalTimeElapsed = 0, totalTime;
    private Image image;

    public Poisson(int x, int y, int width, int height, String type, double remainingTime) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
        this.x_destination = -1;
        this.y_destination = -1;
        this.remainingTime = remainingTime;
	this.type = type;
	this.isStarted = false;
    }

    public void setDestination(int x, int y, double time) {
	this.startX = this.x;
	this.startY = this.y;	
        this.x_destination = x;
        this.y_destination = y;
	this.totalTimeElapsed = 0;
	this.totalTime = time;
        this.remainingTime = time;
    }

    public void startFish(){
	this.isStarted = true;
    }

    // public void updatePosition(double time) {
    //     if (remainingTime > 0) {
    //         double speed = time / remainingTime;
    //         this.x += (this.x_destination - this.x) * speed;
    //         this.y += (this.y_destination - this.y) * speed;
    //         remainingTime -= time;
    //     } else {
    //         this.x = this.x_destination;
    //         this.y = this.y_destination;
    //     }
    // }
public void updatePosition(double time) {
    if (totalTimeElapsed < totalTime) {
        totalTimeElapsed += time;
        double t = Math.min(1.0, totalTimeElapsed / totalTime);
        this.x = (startX + (x_destination - startX) * t);
        this.y = (startY + (y_destination - startY) * t);
        this.remainingTime = totalTime - totalTimeElapsed;
    } else {
        this.x = x_destination;
        this.y = y_destination;
        this.remainingTime = 0;
    }
}    
    public boolean hasReachedDestination() {
        return (this.x == this.x_destination && this.y == this.y_destination);
    }

    public boolean hasStarted() {
	return (this.isStarted);
    }
    
    public void setImage(Image image) {
	this.image = image;
    }
    
    public Image getImage() {
	return image;
    }

    public int getX() { return (int)x; }
    public int getY() { return (int)y; }
    public int getXdest() { return (int)x_destination; }
    public int getYdest() { return (int)y_destination; }    
    public int getWidth() { return width; }
    public int getHeight() { return height; }
    public double getTime() { return remainingTime; }    
    public String getType() { return type; }
}
