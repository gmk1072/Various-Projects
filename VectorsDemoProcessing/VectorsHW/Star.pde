class Star
{
  
  int x,y;//x and y value for the star
  
  Star(int x, int y)
  {
    this.x = x; 
    this.y = y;
  }
  
  void display()
  {
    PShape star = createShape(ELLIPSE,x,y,2,2); //creates a small ellipse at the given RANDMOM x and y 
    shape(star);
  }
  
}