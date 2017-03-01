class Background
{
  
  ArrayList<Star> stars = new ArrayList<Star>();//keeps hold of the actual stars which are the small ellipses being scattered all over
  
  Background()
  {
    int rand = (int)random(140,300);
    for(int i =0;i<rand;i++)//this puts a RANDOM amount of stars RANDOMLY around the screen
    {
      stars.add(new Star((int)random(width),(int)random(height)));
    }
  }
  
  void display()
  {
    for(int i =0;i < stars.size();i++)//displays all the stars
    {
      stars.get(i).display();
    }
  }
  
}