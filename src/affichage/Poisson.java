


public class Fish {
    
    private int x;
    private int y;
    private int x_destination;
    private int y_destination;
    private int width;
    private int height;
    private boolean status;
    private String type;
    private double remainingTime;
    private double totalTime;

    public Fish(int x, int y, int width, int height, String type){
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
        this.type = type;
        this.status = false;
        this.x_destination = 0;
        this.y_destination = 0;
        this.remainingTime = -1;
        this.totalTime = -1;
    }

    public void change_status(boolean status) {
        this.status = status;
    }

    public void updatePosition(double time){
        if (remainingTime > 0) {
            double speed = Math.min(time / remainingTime, 1.0);
            this.x += (this.x_destination - this.x) * speed;
            this.y += (this.y_destination - this.y) * speed;
            remainingTime -= time;
        } else if (remainingTime <= 0) {
            this.x = this.x_destination;
            this.y = this.y_destination;
        }
    }
    
    public void setDestination(int x, int y, double time) {
        this.x_destination = x;
        this.y_destination = y;
        this.totalTime = time;
        this.remainingTime = time;
    }

    public boolean hasReachedDestination() {
        return (this.x == this.x_destination && this.y == this.y_destination);
    }

}
