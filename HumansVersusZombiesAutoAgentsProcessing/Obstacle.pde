class Obstacle
{
  PVector loc;//the location
  float diam;//the diameter of the obstacle
  PShape ell;//the ellipse that is shown in debug mode
  PImage image;//the image
  Obstacle(PVector loc, float radius)
  {
    this.loc = loc;
    this.diam = radius;
    image = loadImage("Tree.png");
    image.resize((int)radius, (int)radius);
  }

  void display()
  {
    ellipseMode(CENTER);
    ell = createShape(ELLIPSE, loc.x, loc.y, diam, diam);
    ell.setFill(255);//sets the circle to be white
    ell.setStroke(0);//set the circle to ahve a black stroke(line)
    if (debug)//if in debug mode shows the circle, otherwise shows the image
    {
      shape(ell);
    } 
    else
    {
      image(image, -diam/2 + loc.x, -diam/2 + loc.y);
    }
  }
}