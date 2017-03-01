ArrayList<Zombie> zombies;//holds the zombies
ArrayList<Human> humans;//holds the humans
boolean debug;
ArrayList<Obstacle> obstacles;
boolean created = false;
boolean humansDebug;
PImage zombie, human, bg;
void setup()
{
  size(1820, 980);
  zombies = new ArrayList<Zombie>();//intitializing and filling the lists for the zombies and humans
  humans = new ArrayList<Human>();
  obstacles = new ArrayList<Obstacle>();
  human = loadImage("Human.png");
  zombie = loadImage("Zombie.png");
  human.resize(50, 50);
  zombie.resize(50, 50);
  bg = loadImage("Grass.jpg");
  bg.resize(width, height);  
  for (int i =0; i < 10; i++)
  {
    zombies.add(new Zombie());
  }
  for (int k =0; k < 10; k++)
  {
    humans.add(new Human());
  }
}

void draw()
{
  if (frameCount > 10 && !created)//for some reason when i made the obstacles at the same time as the humans and zombies, the correct number would not spawn of either zombies or humans. so i am spawning them 10 frames after, and it works.
  {
    for (int j =0; j < 10; j++)
    {
      obstacles.add(new Obstacle(new PVector(random(100, width-100), random(100, height-100)), 50));
    }
    created = true;
  }
  background(255);
  if (!debug)//will have a white background in debug mode and the image of grass when not in debug mode
  {
    image(bg, 0, 0);
  }
  for (int i =0; i<obstacles.size(); i++)//displays each of the obstacles
  { 
    obstacles.get(i).display();
  }
  for (int i =0; i < zombies.size(); i++)//updating and displaying the each of the zombies and humans
  {
    zombies.get(i).update();
    zombies.get(i).display();
  }
  for (int i = (humans.size()-1); i >=0; i--)//updates, displays, removes if necessary each of the humans
  {
    humans.get(i).update();
    if (humans.get(i).zombify)//if the human is set to be zombified, remove it from the list so it wont be called on again rather than displaying it
    {
      println("human " + i + " has been removed");
      println("humans left: " + humans.size());
      println("zombies count: " + zombies.size());
      humans.remove(i);
    } 
    else
    {
      humans.get(i).display();
    }
  }
  if (debug)//if in debug mode draw the human/zombie placement button
  {
    rectMode(CENTER);
    fill(0);
    stroke(0);
    rect(50, 25, 100, 50);
    if (humansDebug)
    {
      fill(255);
      textSize(25);
      text("Human", 6, 30);
      image(human, mouseX-25, mouseY-25);
    } 
    else
    {
      fill(#00FF00);
      textSize(25);
      text("Zombie", 6, 30);
      image(zombie, mouseX-25, mouseY-25);
    }
  }
}

void keyPressed()
{
  if (key == ' ')//pressing space will enter debug mode
  {
    debug = !debug;
  }
}

void mouseReleased()
{
  if (debug)
  {
    if (mouseX < 100 && mouseX > 0 && mouseY < 50 && mouseY > 0)//if clicking the button switches the placement mode for placing either a human or a zombie
    {
      humansDebug = !humansDebug;
    }
    else//clicking any where else(thus you are trying to place a zombie or human)
    {
      if (humansDebug)//place a human at the mouse position
      {
        humans.add(new Human(mouseX, mouseY));
      }
      else//place a zombie at the mouse position
      {
        zombies.add(new Zombie(mouseX, mouseY));
      }
    }
  }
}