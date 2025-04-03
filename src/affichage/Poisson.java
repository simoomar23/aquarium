class Poisson {
    private int x, y, x_destination, y_destination, width, height;
    private double remainingTime, totalTime;
    private String type;

    public Poisson(int x, int y, int width, int height, String type) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
        this.x_destination = 0;
        this.y_destination = 0;
        this.remainingTime = -1;
        this.totalTime = -1;
	this.type = type;
    }

    public void setDestination(int x, int y, double time) {
        this.x_destination = x;
        this.y_destination = y;
        this.totalTime = time;
        this.remainingTime = time;
    }

    public void updatePosition(double time) {
        if (remainingTime > 0) {
            double speed = Math.min(time / remainingTime, 1.0);
            this.x += (this.x_destination - this.x) * speed;
            this.y += (this.y_destination - this.y) * speed;
            remainingTime -= time;
        } else {
            this.x = this.x_destination;
            this.y = this.y_destination;
        }
    }
    public boolean hasReachedDestination() {
        return (this.x == this.x_destination && this.y == this.y_destination);
    }


    public int getX() { return x; }
    public int getY() { return y; }
    public int getWidth() { return width; }
    public int getHeight() { return height; }
    public String getType() { return type; }
}
