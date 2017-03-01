class BoundingBox
{
  //the width, height, x and y position of the bounding box
  float w,h,x,y;
    
  BoundingBox(float x,float y,float w,float h)
  {
    this.w =w;
    this.h =h;
    this.x =x;
    this.y =y;
  }
  
  boolean isColliding(BoundingBox shape)
  {
     boolean one = false; //the first, second, third, and fourth conditions that all have to be true for a collision
     boolean two = false; 
     boolean three = false;
     boolean four =false;
     
       
       if(x < (shape.x + shape.w))//if the first shape is to to the left of the right side of the second shape
       {
        one = true;
       }
       if((x + w )> shape.x)//if the first shape is to the right of the left side of the second shape
       {
        two = true;
       }
       if(y < (shape.y + shape.h))//if the first shape is above the bottom of the second shape
       {
        three = true;
       }
       if((y + h) > shape.y)//if the first shape is below the top of the first shape
       {
        four = true;
       }
       if(one & two && three & four)//it has collided
       {
         song.play(0);
         return true;
       }
     
     
     return false;//it has not collided
  }
  
  void display()
  {
      rect(x,y,w,h);//displays the bounding box
  }
  
}