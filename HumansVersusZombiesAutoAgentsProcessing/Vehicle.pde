abstract class Vehicle 
{  
  PVector acceleration, velocity;  //vectors for moving a vehicle
  PVector forward, right;  //these orientation vectors provide a local point of view for the vehicle
  float x, y, radius, maxSpeed, maxForce, mass;  //floats to describe vehicle movement and size
  Vehicle(float x, float y, float r, float ms, float mf) 
  {
    acceleration = new PVector();
    velocity = new PVector();
    forward = new PVector();
    right = new PVector();
    this.x = x;
    this.y = y;
    radius = r;
    maxSpeed = ms;
    maxForce = mf;
    mass = 1;
  }

  abstract void calcSteeringForces();

  abstract void display();

  void update() 
  {
    calcSteeringForces();    //calculate steering forces by calling calcSteeringForces()
    checkCollisions();    //will check the collisions
    //add acceleration to velocity, limit the velocity, and add velocity to position
    velocity.add(acceleration);
    velocity.limit(maxSpeed);
    x += velocity.x;
    y += velocity.y;
    //calculate forward and right vectors
    forward = velocity.copy().normalize();
    right = forward.copy().rotate(radians(90));
    if (debug)//if its in debug mode draw these line
    {
      //draws a line to represent the forward vector
      stroke(#0000FF);
      line(x, y, (forward.x*50)+x, (forward.y*50)+y);
      //draws a line to represent the right vector
      stroke(#00FF00);
      line(x, y, (right.x*50)+x, (right.y*50)+y);
    }
    //reset acceleration
    acceleration.mult(0);
  }

  void checkCollisions()
  {
    if (humans.size()!=0)//if there are humans
    {
      if (this instanceof Human)//if this vehicle is a human
      {
        for (int i =0; i < zombies.size(); i++)//iterates through the zombies
        {
          if (!((Human)this).zombify)//the human isn't already supposed to be a zombie
          {
            if (PVector.sub(new PVector(zombies.get(i).x, zombies.get(i).y), new PVector(x, y)).mag() < (radius/2 + zombies.get(i).radius/2))//if the distance between the centers of the human and the zombie is less than the radii of each added
            {
              Zombie newZombie = new Zombie();//make a new zombie at the x and y of the human
              newZombie.x = this.x;
              newZombie.y = this.y;
              zombies.add(newZombie);//add that zombie to the list
              ((Human)this).zombify = true;//as a human zombify this vehicle
            }
          }
        }
      }
    }
  }

  PVector seek(PVector target)
  {
    PVector desiredVelocity = new PVector(target.x-x, target.y-y);//the difference between the x's and y's is the directed velocity we want to go
    desiredVelocity.setMag(maxSpeed);//limits the velocity
    return(desiredVelocity.sub(velocity));//returns the difference between the current velocity and the desired one
  }

  PVector pursue(PVector target, float framesAhead, Human human)
  {
    PVector tgt = target.copy();
    PVector desiredVelocity = new PVector((tgt.x + (human.forward.x*framesAhead))-x, (tgt.y + (human.forward.y*framesAhead))-y);//the difference between the x and y of this vehicle and the future position of the target is the direction we want to go
    if (debug)
    {
      stroke(0);
      fill(0);
      line(x, y, PVector.add(tgt, PVector.mult(human.forward, framesAhead)).x, PVector.add(tgt, PVector.mult(human.forward, framesAhead)).y);
      ellipseMode(CENTER);
      ellipse(PVector.add(tgt, PVector.mult(human.forward, framesAhead)).x, PVector.add(tgt, PVector.mult(human.forward, framesAhead)).y, 5, 5);
    }
    desiredVelocity.setMag(maxSpeed);//limits the velocity
    return(desiredVelocity.sub(velocity));//returns the difference between the current velocity and the desired one to steer towards the desired
  }

  PVector flee(PVector target)//same thing as seek, just backwards
  {
    PVector desiredVelocity = PVector.sub(new PVector(x, y), target);
    desiredVelocity.setMag(maxSpeed);
    return(desiredVelocity.sub(velocity));
  }

  PVector evade(PVector target, float framesAhead, PVector velocity)//same thing as pursue, just backwards
  {
    PVector desiredVelocity = PVector.sub(new PVector(x, y), PVector.add(target, PVector.mult(velocity, framesAhead)));
    if (debug)
    {
      stroke(0);
      fill(0);
      line(x, y, PVector.add(target, PVector.mult(velocity, framesAhead)).x, PVector.add(target, PVector.mult(velocity, framesAhead)).y);
      ellipseMode(CENTER);
      ellipse(PVector.add(target, PVector.mult(velocity, framesAhead)).x, PVector.add(target, PVector.mult(velocity, framesAhead)).y, 5, 5);
    }
    desiredVelocity.setMag(maxSpeed);
    return(desiredVelocity.sub(velocity));
  }

  PVector wander()
  {
    PVector center = velocity.copy();
    center.normalize();
    center.mult(radius);
    center.add(new PVector(x, y));

    float rand = random(360);
    PVector target = new PVector(radius/2 * cos(rand), radius/2 * sin(rand));
    target.add(center);
    PVector desiredVelocity = new PVector(target.x-x, target.y-y);//the difference between the x's and y's is the directed velocity we want to go
    desiredVelocity.setMag(maxSpeed);//limits the velocity
    return(desiredVelocity.sub(velocity));//returns the difference between the current velocity and the desired one
  }
  PVector stayInBounds()
  {
    //seeks the center
    return(seek(new PVector(width/2, height/2)));
  }

  PVector avoidObstacle(Obstacle obst, float safeDistance)
  {

    PVector steer = new PVector(0, 0);
    PVector vecToCenter = PVector.sub(obst.loc, new PVector(x, y));
    if (vecToCenter.mag() > safeDistance)
    {
      return(new PVector(0, 0));
    }
    if (PVector.dot(vecToCenter, forward) <0)
    {
      return(new PVector(0, 0));
    }
    float distBetCent = Math.abs(PVector.dot(vecToCenter, right));
    if (distBetCent > (radius/2 + obst.diam/2)+15)
    {
      return(new PVector(0, 0));
    }
    else
    {
      if (PVector.dot(vecToCenter, right) > 0)
      {
        //-
        PVector desiredVelocity = PVector.mult(right, -maxSpeed);
        steer = PVector.mult(PVector.sub(desiredVelocity, velocity), safeDistance/distBetCent);
      } 
      else
      {
        //+
        PVector desiredVelocity = PVector.mult(right, maxSpeed);
        steer = PVector.mult(PVector.sub(desiredVelocity, velocity), safeDistance/distBetCent);
      }
    }
    return steer;
  }
}