class Bullet
{
  BoundingBox box;//the bullets bounding box
  PVector direction,location;//the direction the bullet is heading and the current location of the bullet
  float speed;//the speed of the bullet(constant basically)
  int w;//the width, its a square, so its height too
  boolean dead;//has hit, will not be shown anymore will not detect collision and will be recollected
  
  Bullet(PVector direction, PVector velocity, float x, float y)
  {
    location = new PVector(x,y);
    this.direction = direction.copy();//creates a copy of the direction given(the ships current facing direction)
    speed = velocity.mag() + 3;//the speed is the velocity given(the ships)
    w = 2;//its a 2x2 square
    box = new BoundingBox(location.x,location.y,w,w);//creates bounding box
  }
  void update()
  {
    box.display();//uses the bounding boxes display method to display the bullet rather than creating a rectangle itself
    
    location.add(PVector.mult(direction,speed));//just adds the speed in the direction to the location
    box = new BoundingBox(location.x,location.y,w,w);//makes the bounding box around the location again
    for(int i =0; i < meteors.size();i++)//checks for each of the meteors
    {
      if(box.isColliding(meteors.get(i).box))//if the bullet is colliding with the meteor
      {
        meteors.get(i).breakup = true;//the meteor will break up(which can either be becoming smaller, or getting destroyed)
        dead = true;//breaks the bullet
        ship.score +=10;//adds score since you hit a meteor
      }
    }
    if(!dead)
    {
      checkEdges();//will wrap the bullet if it isn't dead
    }
  }
  
  void checkEdges()
  {
    if(location.x > width)//resets the bullet back zero if it goes of the right of the screen
    {
      location.x = 0;
    }
    if(location.x + 35 < 0)//resets the bullet back to the right of the screen if it goes off the left
    {
      location.x = width;
    }
    if(location.y >height)//resets the bullet back to the top of the screen if it goes off the bottom
    {
      location.y = 0;
    }
    if(location.y + 35 < 0)//resets the bullet back to the bottom of the screen if it goes off the top
    {
      location.y = height;
    }
  }
}