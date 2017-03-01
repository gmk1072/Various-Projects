class Human extends Vehicle
{
  Zombie zombie;//the zombie that the human is currently tracking and running away from
  PVector target;//the target on the window that the human is going to towards to simulate wandering. will only seek it when it isn't fleeing
  PShape body;//the circle that is the human
  float safeArea;//the range that a zombie has to be within to be tracked and fled from
  PVector steeringForce;//the force that acts on the human to affect its steering
  boolean zombify;//whether it will be turned into a zombie or not
  PImage image;//the image of the human
  Human()
  {
    super(random(width-50), random(height-50), 50, 2, .05); 
    steeringForce = new PVector(0, 0);
    ellipseMode(CENTER);
    body = createShape(ELLIPSE, 0, 0, radius, radius);
    safeArea = 200;
    zombify = false;
    image = loadImage("Human.png");
    image.resize((int)radius, (int)radius);
  }

  Human(float x, float y)//for creating a human at a specific x,y (pretty much only used for placing humans)
  {
    super(x, y, 50, 2, .05); 
    steeringForce = new PVector(0, 0);
    ellipseMode(CENTER);
    body = createShape(ELLIPSE, 0, 0, radius, radius);
    safeArea = 200;
    zombify = false;
    image = loadImage("Human.png");
    image.resize((int)radius, (int)radius);
  }

  void calcSteeringForces()
  {
    float distance = safeArea +1;//initializes the distance that a zombie could be at far enough to not be tracked
    if (zombies.size()!=0)//if there are no zombies, shouldn't happen but just in case
    {
      zombie = zombies.get(0);//starts at the first zombie

      distance = PVector.sub(new PVector(zombie.x, zombie.y), new PVector(x, y)).mag();//gets the distance from the human to the first zombie
      for (int i = 1; i<zombies.size(); i++)//goes through the list of zombies
      {
        float distance2 = PVector.sub(new PVector(zombies.get(i).x, zombies.get(i).y), new PVector(x, y)).mag();//gets the distance from the human to the zombie at index i
        if (distance2 <= distance)//compares the current shortest distance with the distance to the zombie at index i
        {
          zombie = zombies.get(i);//if the ith zombie is closer sets the current zombie being tracked to the new one
          distance = distance2;//collects the new shortest distance
        }
      }
    }



    //instantiates each of the steering force vectors
    PVector e = new PVector(0, 0);
    PVector w = new PVector(0, 0);
    PVector sib = new PVector(0, 0);
    PVector a = new PVector(0, 0);
    //if the closest zombie is within the flee distance(safeArea)
    if (distance <= safeArea)
    {
      //gets the evade force on that zombie
      e= evade(new PVector(zombie.x, zombie.y), 50, zombie.velocity);
    }
    else//there isn't a close enough zombie, start wandering
    {
      w = wander();
    }
    if (x > width-50 || y > height-50 || x < 50 || y < 50)//if the human is ouside of bounds
    {
      //makes the stay in bounds force take priority
      e.mult(.5);
      w.mult(0);
      sib = stayInBounds();
    }
    for (int i =0; i < obstacles.size(); i++)//avoids any obstacles within range
    {
      if (PVector.sub(obstacles.get(i).loc, new PVector(x, y)).mag() <= safeArea)
      {
        a = avoidObstacle( obstacles.get(i), safeArea);
      }
    }
    if (a.mag() >0)//if there is an obstacle avoidance force evasion force is decreased and wander force is non existant
    {
      w.mult(0);
      e.mult(.1);
    }
    a.mult(500);//fullpriority to evasion force
    //adds all the forces to the steeringForce
    steeringForce.add(e);
    steeringForce.add(w);
    steeringForce.add(sib);
    steeringForce.add(a);
    if (e.mag() == 0)//if there is no evasion force(no zombies around) slower maxSpeed
    {
      maxSpeed = 1;
    } 
    else
    {
      maxSpeed = 2;
    }
    steeringForce.limit(maxForce);//limits the steeringForce
    acceleration.add(steeringForce);//adds the steeringForce to the acceleration 
    steeringForce.mult(0);//resets the steeringForce
  }

  void display()
  {
    float angle = velocity.heading();//gets the direction of the human
    body.setFill(#FFFF00);//sets the circle that is the human in debug mode to yellow
    body.setStroke(0);//sets the stroke(line) of the circle to black
    //moves and rotates the human
    pushMatrix();
    translate(x, y);
    rotate(angle);
    if (debug)//draws either the circle or the image depending on whether its in debug mode or not
    {
      shape(body);
    } 
    else
    {
      image(image, -radius/2, -radius/2);
    }
    popMatrix();
  }
}