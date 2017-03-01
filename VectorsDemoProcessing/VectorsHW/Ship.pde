//class Ship
//Purpose: represents the ship that the player controls
class Ship
{
  PVector acceleration, velocity, direction, location;//declaring the acceleration, velocity, direction and location vectors for movement
  float mass, speed, maxSpeed;//holds the mass(essentially the size of the ship), the speed and the maxspeed(the limit of the velocity magnitude)
  BoundingBox box;//the collision detection box
  boolean wPressed, aPressed, dPressed, sPressed,invincible,dead,pressed,canShoot;//booleans for the key presses(w,a,s,d, and space) whether the ship is invincible at the moment(occurs after spawn and taking damage), and whether you are dead(for Game over)
  ArrayList<Bullet> bullets;//list of bullets to call update on them and to limit the amount you have out at one time
  PShape ship;//the ship shape
  int hits,invincibleCount,counter;//the number of hits(lives removed), the time counter for invincibility, the time counter for the interval in between shots
  int score;//the score
  
  Ship()
  {//setting all of the variables
    acceleration = new PVector(0,0);
    velocity = new PVector(0,0);
    velocity.limit(15);
    direction = PVector.fromAngle(radians(270));//starts the ship facing up
    location = new PVector(width/2,height/2);//starts in the middle
    mass = 15;//not really used, just in the size of this i guess
    speed = 0;//the speed starts at 0
    maxSpeed = 3;//can only get up to 3
    bullets = new ArrayList<Bullet>();//the bullets
    box = new BoundingBox(location.x,location.y,mass,mass);
    hits = 0;
    invincible = true;//starts the ship as invincible for a couple seconds
    canShoot = true;//can shoot the lasers
  }
  
  void rotateShip()
  {
    if(aPressed)//rotate the direction for new forces and velocity for current forces
    {
      direction.rotate(-.02);
      velocity.rotate(-.02);
    }
    if(dPressed)//rotate the direction for new forces and velocity for current forces
    {
      direction.rotate(.02);
      velocity.rotate(.02);
    }
  }
    
  
  void applyForce(PVector force)
  {
    PVector f = PVector.div(force,mass);//adds the appropriate force
    acceleration.add(f);
  }
  void update()
  {
    if(wPressed)//when 'w' is pressed apply the forward thrust in the correct direction
    {
      song2.play();//plays the thruster noise
      applyForce(PVector.mult(direction,.2));
    }
    if(sPressed)//since the ship is in a vacuum and no forces are acting upon it in opposition you have to press 's' to place a stopping thrust on the ship
    {
      velocity.mult(.96);
    }
    keyPressed();//calls the keypressed and keyreleased functions to check for either of those events
    keyReleased();
    checkEdge();//will check the edges to make sure the ship wraps around
    rotateShip();//will check if the 'a' or 'd' keys are being pressed to make the ship rotate
    velocity.add(acceleration);//adds the acceleration to the velocity to go in the specific direction
    velocity.limit(maxSpeed);//makes sure the ship doesn't go too fast, don't want to lose the reentry isolation now would you?
    location.add(velocity);//moves the ship according to the velocity
    acceleration.mult(0);//makes the forces added on to the ship the actual acting forces in moving the ship
    box= new BoundingBox(location.x,location.y,mass,mass);//updates the bounding box so the box is around the ship at all times
    //box.display();
    
    for(int i =0;i < meteors.size();i++)//iterates through the list of meteors
    {
      if(box.isColliding(meteors.get(i).box))//checks if ship is colliding with each of the meteors
      {
          meteors.get(i).breakup = true;//will break the meteor
          if(!invincible)//if you are invincible you dont want count hits
          {
            hits++;
            invincible = true;//will make the ship invincible after getting hit
          }            
          if(hits > 3)//if you get hit too many times(4) you die, game over
          {
            die();
          }
        }
    }
    for(Bullet b: bullets)//iterates through the list of bullets
    {
      if(!b.dead)//if the bullet is active
      {
        b.update();//will update the bullet(movement, collision detection, wrapping and so on
      }
    }
    if(pressed ==true)//the space bar has been pressed and cannot be activated for a time now
    {
      if(canShoot)
      {
//with your space age technology you retrieve your used ammo if it has not connected with its target yet. there is a limitation and the weapons system can only 
//keep track of 6 bullets at a time, so it has to retrieve the first bullet fired that hasnt hit.
        if(bullets.size() > 5) 
        {
          song5.play();//plays the laser noise
          bullets.remove(bullets.size()-1);//removes the first bullet fired that hasn't hit anything or already hit
          bullets.add(0, new Bullet(direction,velocity,direction.x + location.x + mass/2,direction.y + location.y + mass/2));//adds a new bullet to the beginning of the list
        }
        else
        {
          song5.play();//plays the laser noise
        bullets.add(0,new Bullet(direction,velocity,direction.x + location.x + mass/2,direction.y + location.y + mass/2));//there isn't more than 6 bullets in the list->adds a new bullet to the list and fires
        }
        song5.rewind();//resets the laser noise
        canShoot = false;//cannot shoot, on cooldown
      }
      if(!canShoot)//if you cant shoot start the cooldown timer
      {
        counter++;//will start counting to eventually reset the space bar to be pressed again
        if(counter>frameRate*.3)//when its been a little less than a second(around 1/3 of a second)
        {
          counter = 0;//resets the counter
          canShoot = true;//can shoot now
        }
      }
    }
    
  }
  //i could not find another way to go about this but, i am using booleans to track multpile key presses at one time
  //seems to get a little glitchy at detecting 3 or more key presses. probably a limitation of processing or ghosting on the keyboard(although it seems my keyboard can detect up to 5 simultaneous key presses) otherwise i have no idea
  void keyPressed()
  {
    //when one key is pressed the boolean is true and it is detected later that when the boolean is true that key is pressed
    if(key =='w')
    {
      wPressed = true;
    }
    if(key=='s')
    {
      sPressed = true;
    }
    if(key =='a')
    {
      aPressed = true;
    }
    if(key =='d')
    {
      dPressed = true;
    }
    if(key == ' ')
    {
      pressed = true;
    }
  }
  void keyReleased()
  {
    //it has to go back to being false however otherwise the key will always be pressed. so i reset all the booleans back to false here to release that key and stop doing the associated action.
    if(char(keyCode) =='A')
    {
      aPressed = false;
    }
    if(char(keyCode)=='D')
    {
      dPressed = false;
    }
    if(char(keyCode)=='S')
    {
      sPressed = false;
    }
    if(char(keyCode)=='W')
    {
      song2.pause();//stops the thrusters
      song2.rewind();
      wPressed = false;
    }
    if(char(keyCode) == ' ')
    {
      pressed = false;
    }
  }
  //wraps the ship around screen
  void checkEdge()
  {
    if(location.x > width)
    {
      location.x = 0;
    }
    if(location.x + 35 < 0)
    {
      location.x = width;
    }
    if(location.y >height)
    {
      location.y = 0;
    }
    if(location.y + 35 < 0)
    {
      location.y = height;
    }
  }
    
  void die()
  {
    dead = true;//self-explanatory
    //for(int i = 0 ; i <50;i ++)
    //{
    //  PVector dir = new PVector(random(360)-180,random(360)-180);
    //  dir.normalize();
    //  new Bullet(dir,new PVector(
    //}
    
  }
  void display()
  {//displays the ship at the location with a certain "mass"
    PShape ell = createShape(ELLIPSE,location.x+mass/2,location.y+mass/2,mass,mass);//the main part of the ship
    PVector dir = PVector.mult(direction,-1);//direction opposite the ships
    PShape ell2 = createShape(ELLIPSE,dir.x * mass*.6 + location.x + mass/2,dir.y*mass*.6 + location.y+mass/2,mass/2,mass/2);//the shape following the main part of the ship, you could say its the thrusters
    PShape ell3 = createShape(ELLIPSE,dir.x * mass*.8 + location.x + mass/2,dir.y*mass*.8 + location.y + mass/2,mass/4,mass/4);//the "flame" for the thrusters
    ell3.setFill(#FFFF00);//sets colors
    ell3.setStroke(#FFFF00);
    ell.setStroke(255);
    ell2.setStroke(255);
    if(invincible)//if the character is invincible
    {
      invincibleCount++;//counting how long the player is invinvible to track when it should run out
      //ellipse(location.x,location.y,mass,mass);
      ell.setFill(#ff0000);//makes the character look red to show that you are invincible
      ell2.setFill(#ff0000);
      ell.setStroke(#FF0000);
      ell2.setStroke(#FF0000);
      if(invincibleCount>2.5*frameRate)//about 2.5 seconds later the invincibility will run off
      {
        invincibleCount = 0;//resets counter
        invincible = false;//no longer invincible
      }
    }
    //ellipse(location.x,location.y,mass,mass);
    shape(ell);//places shapes
    shape(ell2);
    if(wPressed)//if you are thrusting, show the flame
    {
      shape(ell3);
    }
    
  }
}