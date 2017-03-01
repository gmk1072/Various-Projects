import ddf.minim.*;//audio
Minim minim;
PFont font;//font
AudioPlayer song;//explosion
AudioPlayer song2;//thrusters
AudioPlayer song3;//song
AudioPlayer song4;//boop
AudioPlayer song5;//laser
boolean music;//whether to play the music or not
Ship ship;//the player ship
ArrayList<Meteor> meteors;//list of the meteors 
Background bg;//the stars
PImage soundOn; //if the music is on
PImage soundOff;//if the music is off
int counter;
boolean count;
enum GameState//determining which state the game is in, in the main menu, in the game or in the instructions screen
{
  MAINMENU,GAME,INSTRUCTIONS
};
GameState gState;//the actual GameState object
void setup()
{
  size(1000,1000);
  minim = new Minim(this);//sets up the minim audio thingy
  song = minim.loadFile("explosion.wav");//the explosion sound for when things collide with meteors
  song.setGain(-20);//sets the gain for the sound
  song2 = minim.loadFile("thrusters.wav");//the thrusters for when the ship is moving forward
  song2.setGain(-15);
  song3 = minim.loadFile("Whoever Brings The Night.mp3");//the song
  song3.setGain(-10);
  song4 = minim.loadFile("boop.wav");//the boop in the menus
  song5 = minim.loadFile("laser.wav");//the laser noise for shooting
  song5.setGain(-30);
  song4.setGain(-20);
  ship = new Ship();
  soundOn = loadImage("SoundOn.png");
  soundOff = loadImage("SoundOff.png");
  bg = new Background();
  meteors = new ArrayList<Meteor>();
  font = createFont("heavy_data.ttf",12);//creates the font at the set size which doesnt matter because it gets over ridden later
  for(int i =0; i <10;i++)//loops to add 10 meteors to the meteors list to start
  {
    meteors.add(new Meteor());
  }
  gState = GameState.MAINMENU;//starts the game off at the main menu
  music = true;
}

void draw()
{
  background(0);//black background(redundant, but left it in in case it gets laggy when determining the state of the game
  textFont(font);
  if(music)
  {
    song3.play();
  }
  if(gState == GameState.MAINMENU)//if the game is in the mainmenu state will output the following for that
  {
     background(0);
     fill(255);//white text for meteors
     textSize(100);//big text size
     text("METEORS", width/4 + 50,300 );
     fill(#00ff00);//green background for button
     rect(width/2-50, height/2-25,100,50);
     fill(0);
     textSize(30);
     text("START", width/2-37,height/2+10); 
     fill(#00ff00); //<>//
     rect(width/2-125, height/2+50,250,50);
     fill(0);
     textSize(37);
     text("INSTRUCTIONS", width/2-110,height/2+85); 
     if(music)
     {
       soundOn.resize(30,30);
       image(soundOn,width-50,20);
     }
     else
     {
       soundOff.resize(30,30);
       image(soundOff,width-50,20);
     }
     if(count)
     {
       counter++;
     }
     if(counter>frameRate*.1)
     {
       counter = 0;
       count = false;
     }
     if(mousePressed)//detects if the mouse has been pressed, if it has will check the location of where it was pressed
     {
       if(mouseX > width/2-50 && mouseX < width/2-50 + 100 && mouseY > height/2-25 && mouseY < height/2-25 + 50)
       {
         song3.pause();//will pause the music to rewind and set the gain
         song3.rewind();
         song3.setGain(-25);
         song4.play();//will play the boop noise because the button has been pressed
         gState = GameState.GAME;//will now be in the GAME game state
       }
       if(mouseX > width/2-125 && mouseX < width/2-125 +250 && mouseY > height/2+50 && mouseY < height/2+50 + 50)
       {
         song4.play();
        gState = GameState.INSTRUCTIONS;//will now be in the INSTRUCTIONS game state
       }
       
       if(count == false)
       {
         if(mouseX > width - 50 && mouseX < width - 50 + 30 && mouseY > 20 && mouseY < 20 + 30)
         {
           song4.play();
           song3.pause();
           song3.rewind();
           music = !music;
           count = true;
         }
       }
       
       song4.rewind();//rewinds the boop noise
     }
     if(music)
     {
       if(!song3.isPlaying())//will loop the music
       {
         song3.rewind();
         song3.play();
       }
     }
  }
  if(gState == GameState.GAME)
  {
    background(0);//makes the background as black as the night sky
    bg.display();
    if(!ship.dead)
    {
      ship.update();
      ship.display();
      textSize(14);
      fill(255);
      text("ships health:", width-220,10);//the health bar label
      text("score: " + (ship.score), width-100,25); //the score
      rect(width-125,0,100-ship.hits*25,10);//the health bar
    }
    else
    {
      fill(255);
      textSize(100);
      if(music)
      {
        song3.play();
      }
      text("Game Over",width/4,height/2);
      textSize(40);
      text("your score was: " + ship.score,(width/4)+90,height/2 + 80);
      fill(#00ff00);
     rect(width/2-50, height/2+100,120,50);
     fill(0);
     textSize(26);
     text("RESTART", width/2-37,height/2+135); 
     if(mousePressed)
     {
       if(mouseX > width/2-50 && mouseX < width/2-50 + 120 && mouseY > height/2+100 && mouseY < height/2+100 + 50)
       {
         song4.play();//boop
         song.pause();
         song2.pause();
         song3.pause();
         restart();
       }
       song4.rewind();
       
     }
    }
    for(int i =0;i<meteors.size();i++)
    {
      if(!meteors.get(i).dead)//if the meteor has not been destroyed
      {
        meteors.get(i).update();//update and display that not destroyed meteor
        meteors.get(i).display();
      }
      else
      {
        //if(meteors.size()<20)
        //{
          meteors.get(i).reset();//will reset the meteor(spawn a new one)
        //}
      } 
    }
    if(music)
    {
      if(!song3.isPlaying())//loops the music
       {
         song3.rewind();
         song3.play();
       }
    }
  }
  if(gState == GameState.INSTRUCTIONS)
  {
    fill(255);
    textSize(25);
    text("Instructions:",10,80);
    text("you move forward with the 'W' key and stop with the 'S' key",10,130);
    text("to turn you use 'A' for left and 'D' for right", 10,180);
    text("you can shoot with the SPACE BAR", 10, 230);
    text("each meteor will give you points when you destroy so",10,280);
    text("try to get as many as possible before all your lives run out", 40,310);
    fill(#00ff00);
     rect(width/2-50, height/2+100,100,50);
     fill(0);
     textSize(33);
     text("BACK", width/2-37,height/2+135); 
     if(mousePressed)
     {
       if(mouseX > width/2-50 && mouseX < width/2-50 + 100 && mouseY > height/2+100 && mouseY < height/2+100 + 50)
       {
         song4.play();//boop
         gState = GameState.MAINMENU;//now in the MAINMENU game state
       }
       song4.rewind();
       
     }
     if(music)
     {
       if(!song3.isPlaying())
       {
         song3.rewind();
         song3.play();
       }
     }
  }
}

void restart()
{
  minim = new Minim(this);//sets up the minim audio thingy
  song = minim.loadFile("explosion.wav");//the explosion sound for when things collide with meteors
  song.setGain(-20);//sets the gain for the sound
  song2 = minim.loadFile("thrusters.wav");//the thrusters for when the ship is moving forward
  song2.setGain(-15);
  song3 = minim.loadFile("Whoever Brings The Night.mp3");//the song
  song3.setGain(-10);
  song4 = minim.loadFile("boop.wav");//the boop in the menus
  song5 = minim.loadFile("laser.wav");//the laser noise for shooting
  song5.setGain(-30);
  song4.setGain(-20);
  ship = new Ship();
  soundOn = loadImage("SoundOn.png");
  soundOff = loadImage("SoundOff.png");
  bg = new Background();
  meteors = new ArrayList<Meteor>();
  font = createFont("heavy_data.ttf",12);//creates the font at the set size which doesnt matter because it gets over ridden later
  for(int i =0; i <10;i++)//loops to add 10 meteors to the meteors list to start
  {
    meteors.add(new Meteor());
  }
  gState = GameState.MAINMENU;//starts the game off at the main menu
  music = true;
}