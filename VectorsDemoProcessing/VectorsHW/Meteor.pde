class Meteor
{
  BoundingBox box;//its bounding box
  boolean entered;//it has entered the screen so it will wrap now(makes it so it doesn't get trapped out of the screen)
  PVector direction;//the direction it is heading
  PVector location;//its current location
  boolean breakup;//whether it will break up or not(die or become smaller)
  boolean dead;//die
  int w,h;//width and height
  PImage rock;//the image of the meteor
  Meteor()
  {
    PVector spawn = new PVector(random(width + 200)-100,random(height + 200)-100);//spawn somewhere out of the screen or within the screen
    do
    {
      spawn = new PVector(random(width + 200)-100,random(height + 200)-100);
    }
    while(spawn.x < width && spawn.x > 0 && spawn.y > 0 && spawn.y < height);//will not keep that spawn point if it is within the screen
    
    w = (int)random(30,100);//random width and height
    h = (int)random(30,100);
    box = new BoundingBox(spawn.x,spawn.y,w-5,h-5);//sets up the bounding box
    location = spawn.copy();//gets that location from the spawn
    entered = false;//it hasnt entered the screen yet
    direction = PVector.sub(new PVector(random(width),random(height)),location);//will head towards a random location on the screen
    direction.normalize();//normalizes the direction vector
    dead = false;//it's not dead yet
    rock = loadImage("meteor.png");//loads the actual image
    rock.resize(w,h);//resizes the image to fit according to the width and height
  }
  
  //Meteor(float x, float y, int w, int h, PVector direction)//
  //{
  //  entered = true;
  //  this.direction = direction.copy();
  //  location = new PVector(x,y);
  //  this.w = w;
  //  this.h = h;
  //  dead = false;
  //  box = new BoundingBox(location.x,location.y,w-5,h-5);
  //  rock = loadImage("meteor.png");
  //  rock.resize(w,h);
  //}
  
  void update()
  {
    location.add(PVector.mult(direction,1));//adds the "speed" onto the location in the direction

    if(entered ==false)//if it hasnt entered yet, checks if it has
    {
      if(location.x > 0 && location.x < width && location.y >0 && location.y < height)
      {
        entered = true;
        
      }
    }
    else//it has entered
    {
      checkEdges();//will now wrap
    }
    if(breakup)//if it has been hit and should now break up
    {
      if(w>=60||h>=60)//if it is big enough will just shatter rather than be destroyed
      {
         w/=2;//makes it half the size
         h/=2;
         box.w/=2;//makes the bounding box according to the new dimensions
         box.h/=2;
         direction.rotate(random(360));//rotates the direction randomly when its been hit
         rock.resize(w,h);//resizes the picture to the new dimensions
         //Meteor mini = new Meteor(location.x+ w,location.y+h,w,h,direction);
         //meteors.add(mini);
         //direction.rotate(random(360));
         breakup = false;//will have to be broken again
      }
      else//get wrekt
      {
        dead = true;
      }
    }
    box= new BoundingBox(location.x,location.y,w-5,h-5);//changes the bounding box with the new width, height,and location
    //box.display();
  }
  
  void display()
  {
    image(rock,location.x,location.y);//displays image
  }
  
  
  void checkEdges()//wraps the meteor
  {
    if(location.x > width)
    {
      location.x = 0;
    }
    if(location.x + 35 < 0)
    {
      location.x = width-w;
    }
    if(location.y >height)
    {
      location.y = 0;
    }
    if(location.y + 35 < 0)
    {
      location.y = height-h;
    }
  }
  void reset()//creates a "new" meteor by just manipulating values rather than making a new instance
  {
    PVector spawn = new PVector(random(width + 200)-100,random(height + 200)-100);
    do
    {
      spawn = new PVector(random(width + 200)-100,random(height + 200)-100);
    }
    while(spawn.x < width && spawn.x > 0 && spawn.y > 0 && spawn.y < height);
    
    //spawn = new PVector(100,100);
    w = (int)random(30,100);
    h = (int)random(30,100);
    box = new BoundingBox(spawn.x,spawn.y,w-5,h-5);
    location = spawn.copy();;
    entered = false;
    direction = PVector.sub(new PVector(random(width),random(height)),location);
    direction.normalize();
    dead = false;
    rock = loadImage("meteor.png");
    rock.resize(w,h);
    breakup = false;
  }
}