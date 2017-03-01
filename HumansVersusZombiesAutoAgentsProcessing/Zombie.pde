class Zombie extends Vehicle
{
  Human human;//the human that the zombie is currently tracking and seeking
  PShape body;//the circle that is the zombie in debug mode
  PVector steeringForce;//the force that acts on the zombie to affect its steering
  PImage image;//the image of the zombie
  Zombie()
  {
    super(random(width-50), random(height-50), 50, 1, .01);
    steeringForce = new PVector(0, 0);
    ellipseMode(CENTER);
    body = createShape(ELLIPSE, 0, 0, 50, 50);
    image = loadImage("Zombie.png");
    image.resize((int)radius, (int)radius);
  }

  Zombie(float x, float y)
  {
    super(x, y, 50, 1, .01);
    steeringForce = new PVector(0, 0);
    ellipseMode(CENTER);
    body = createShape(ELLIPSE, 0, 0, 50, 50);
    image = loadImage("Zombie.png");
    image.resize((int)radius, (int)radius);
  }

  void calcSteeringForces()
  {
    PVector p = new PVector(0, 0);//the pursue force
    PVector sib = new PVector(0, 0);//the stay in bounds force
    PVector a = new PVector(0, 0);//avoid obstacle force force
    PVector w = new PVector(0, 0);//the wander force
    if (humans.size()>0)//if there are humans                                                                                                                                              
    {                                                                                                                                                               
      human = humans.get(0);//gets the first human in the list                                                                                                                                             
      float distance = PVector.sub(new PVector(human.x, human.y), new PVector(x, y)).mag();//gets the distance from that human to this zombie                                                                    
      for (int i = 1; i < humans.size(); i++)                                                                                                           
      {                                                                                                                                                           
        float distance2 = PVector.sub(new PVector(humans.get(i).x, humans.get(i).y), new PVector(x, y)).mag();//gets the ditsance from this zombie to the ith human                                                                                                           
        if (distance2 <= distance)//compares the distances                                                                                                                                  
        {                                                                                                                                                           
          human = humans.get(i);//collects the closest human                                                                                                                                          
          distance = distance2;//keeps the shortest distance
        }
      }            
      sib = new PVector(0, 0);
      p = pursue(new PVector(human.x, human.y), 50, human);//seeks that closest human
    }
    else//there are no humans, so wander
    {
      w = wander();
    }
    if (x > width-50 || y > height-50 || x < 50 || y < 50)//stays in bounds if out of bounds
    {
      p.mult(0);//gives focus to the stay in bounds force
      sib = stayInBounds();
    }
    for (int i =0; i < obstacles.size(); i++)//avoids the obstacles if they are within the safeArea
    {
      if (PVector.sub(obstacles.get(i).loc, new PVector(x, y)).mag() <= 100)
      {
        a = avoidObstacle( obstacles.get(i), 100);
      }
    }
    if (a.mag() >0)//if there is an avoidance force dont wander, or pursue anymore and avoid the obstacle
    {
      w.mult(0);
      p.mult(0);
    }
    if (p.mag() > 0)//if the zombie is not pursuing, it moves slower
    {
      maxSpeed = 1;
    } 
    else
    {
      maxSpeed = .3;
    }
    a.mult(500);//high priority to avoiding obstacles
    //adds each of the steering forces, seek and stay in bounds, to the steeringForce
    steeringForce.add(p);
    steeringForce.add(sib);
    steeringForce.add(a);
    steeringForce.add(w);
    steeringForce.limit(maxForce);//limits that force
    acceleration.add(steeringForce);//adds the steeringForce to acceleration
    steeringForce.mult(0);//resets the steeringForce
  }

  void display()
  {
    float angle = velocity.heading();//the direction of the zombie
    //sets the zombie to green and the stroke(line) to black
    body.setFill(#00FF00);
    body.setStroke(0);
    //moves and rotates the zombie
    pushMatrix();
    translate(x, y);
    rotate(angle);
    if (debug)
    {
      shape(body, 0, 0);
    } 
    else
    {
      image(image, -radius/2, -radius/2);
    }
    popMatrix();
  }
}