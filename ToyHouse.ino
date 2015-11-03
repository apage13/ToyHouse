#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include "SwitchMonitor.h"

//Definitions for sound tracks
#define SND_BRIGHT                      1
#define SND_LIGHTS_ON                   2
#define SND_DARK                        3
#define SND_LIGHTS_OFF                  4

#define SND_SHORT_DOORBELL              5
#define SND_LONG_DOORBELL               6
#define SND_LETS_PRETEND_ITS_MORNING    7 
#define SND_LETS_PRETEND_ITS_LUNCHTIME  8
#define SND_LETS_PRETEND_ITS_NIGHTTIME  9
#define SND_SUN                         10
#define SND_MOON                        11
#define SND_NIGHT                       12
#define SND_DAY                         13
#define SND_OPPOSITES                   14
#define SND_LITTLE_BIRD_DOWN            15
#define SND_BIG_BIRD_DOWN               16
#define SND_SOFT_TWEET                  17
#define SND_LOUD_TWEET                  18
#define SND_QUACK_LONG                  19
#define SND_QUACK_SHORT                 20
#define SND_QUACK_LOUD                  21
#define SND_QUACK_SOFT                  22

//do not change order of the following
#define SND_BRUSH_TEETH_MORNING         23    //rand InMorningMode
#define SND_GOOD_MORNING                24    //rand InMorningMode
#define SND_WASH_YOUR_HANDS_LUNCHTIME   25    //rand afternoon
#define SND_QUACK_SOFT_2                26    //rand afternoon or rand nighttime
#define SND_BRUSH_TEETH_BEDTIME         27    //rand nighttime
#define SND_TAKING_A_BATH               28    //rand nighttime
#define SND_FULL_OF_BUBBLES             29    //rand nighttime
#define SND_TURN_ON_THE_WATER           30    //rand nighttime  
#define SNG_LITTLE_RUBBER_DUCK_SONG     31    //rand nighttime

#define SND_OPEN                        40
#define SND_CLOSE                       41 
#define SND_WHERE_IS_THE_CHICKEN        42  
#define SND_WHERE_IS_THE_MILK           43
#define SND_BRR_ITS_TOO_COLD            44 
#define SND_LETS_COOK                   45

#define SND_SUN_IS_UP                   50
#define SND_STORY1                      51
#define SND_STORY2                      52
#define SND_STORY3                      53
#define SND_STORY4                      54
      
//Definitions for house inputs and outputs
//0 and 1 are reserved for the serial port
#define INPUT_FRIDGE_DOOR  2
#define INPUT_BOOK         3  //single state only?
#define INPUT_DUCK         4  //single state only?
#define INPUT_LIGHT_SWITCH 5
#define INPUT_BIRDS        6
#define INPUT_DOORBELL     7  //single state only?
#define INPUT_MODE         8
#define INPUT_WINDOW       9

#define INPUT_BUSY_SIGNAL  10

#define OUTPUT_LAMP        11


SwitchMonitor BookButton;
int LastBookPressedSound;

SwitchMonitor DuckButton;
int LastDuckPressedSound;

SwitchMonitor DoorbellButton;
int LastDoorBellSound;

SwitchMonitor LightSwitch;
int LastLightSwitchSound;

SwitchMonitor LittleBird;
int LastBirdSound;

SwitchMonitor FridgeDoor;
int LastFridgeDoorSound;

SwitchMonitor WindowSwitch;
int LastWindowSound;

SwitchMonitor ModeSwitch;

bool OppositeModeSelected;
bool DaytimeWindowSelected;
bool InMorningMode;
bool PlayingSong;

void PlaySound(int soundNumber)
{
  //If playing a song, we ignore other sound requests until the song completes
  if (PlayingSong)
    return;

  switch (soundNumber)
  {
    case SNG_LITTLE_RUBBER_DUCK_SONG:
      PlayingSong = true;       
      Serial.println("Playing Song");
      break;
    default:
      PlayingSong = false;
      break;
  }
  
  //Send track to play to sound module  
  //mp3_play (soundNumber);
  //For debugging for now, print out the song number we wanted to play
  Serial.print("Playing Sound: ");
  Serial.println(soundNumber);
  delay(100); //Wait 100ms before moving on to make sure the sound has started playing and busy signal is high
}

void setup()
{
  //Initialize serial port for sound module
  Serial.begin (9600);
  //mp3_set_serial (Serial);      //set Serial for DFPlayer-mini mp3 module 
  //delay(1);                     // delay 1ms to set volume
  //mp3_set_volume (1);          // value 0~30

  BookButton.Init(INPUT_BOOK);
  DuckButton.Init(INPUT_DUCK);
  DoorbellButton.Init(INPUT_DOORBELL);
  FridgeDoor.Init(INPUT_FRIDGE_DOOR);
  LightSwitch.Init(INPUT_LIGHT_SWITCH);
  LittleBird.Init(INPUT_BIRDS);
  WindowSwitch.Init(INPUT_WINDOW);
  ModeSwitch.Init(INPUT_MODE);
  
  //Make sure lamp output is off
  digitalWrite(OUTPUT_LAMP, false);
}

void loop()
{    
  //Check status of sound module to determine when song is finished
  if (PlayingSong && digitalRead(INPUT_BUSY_SIGNAL) == LOW)
  {
      //When busy signal drops, set PlayingSong to false
      PlayingSong = false;  
  }

  //Check our inputs for any that changed state

  //Rubber Duck
  if (DuckButton.StateChanged() && DuckButton.On)
    ExecuteDuckButtonPressed();

  //Doorbell
  if (DoorbellButton.StateChanged() && DoorbellButton.On)
    ExecuteDoorbellPressed();

  //Book
  if (BookButton.StateChanged() && BookButton.On)
    ExecuteBookPressed();

  //Refrigerator Door
  if (FridgeDoor.StateChanged())
    ExecuteFridgeDoorChanged(FridgeDoor.On);

  //Light Switch
  if (LightSwitch.StateChanged())
    ExecuteLightSwitchChanged(LightSwitch.On);

  //Birds
  if (LittleBird.StateChanged())
    ExecuteBirdsChanged(LittleBird.On);

  //Window
  if (WindowSwitch.StateChanged())
    ExecuteWindowChanged(WindowSwitch.On);
  DaytimeWindowSelected = WindowSwitch.On;

  //Mode
  if (ModeSwitch.StateChanged())
    ExecuteModeChanged(ModeSwitch.On);
  OppositeModeSelected = ModeSwitch.On;
}

void ExecuteDuckButtonPressed()
{  
  Serial.println("Duck Pressed");
  if (OppositeModeSelected)
  {
    //Scroll through opposite pairs
  }
  else
  {
    if (DaytimeWindowSelected)
    {
      if (InMorningMode)
      {
        //Random pick from daytime sounds
        PlaySound(random(SND_QUACK_SOFT, SND_WASH_YOUR_HANDS_LUNCHTIME+1));
      } 
      else //afternoon
      {
        //Random pick from afternoon sounds
        PlaySound(random(SND_WASH_YOUR_HANDS_LUNCHTIME, SND_BRUSH_TEETH_BEDTIME+1));
      }
    }
    else //nighttime
    {
      //Random pick from nighttime sounds 
      PlaySound(random(SND_QUACK_SOFT_2, SNG_LITTLE_RUBBER_DUCK_SONG+1)); 
    }
  }
}

void ExecuteDoorbellPressed()
{  
  Serial.println("Doorbell Pressed");
}

void ExecuteBookPressed()
{  
  Serial.println("Book Pressed");
}

void ExecuteFridgeDoorChanged(bool doorOpen)
{
  Serial.print("Fridge Door Changed - Door ");
  Serial.println((doorOpen ? "Open" : "Closed"));
}

void ExecuteLightSwitchChanged(bool lightOn)
{
  Serial.print("Light Switch Changed - Light ");
  Serial.println((lightOn ? "On" : "Off"));
  
  if (OppositeModeSelected)
  {
    if (lightOn)
    {
      LastLightSwitchSound = random(SND_BRIGHT, SND_LIGHTS_ON+1);
      PlaySound(LastLightSwitchSound);
    }
    else
    {
      switch (LastLightSwitchSound)
      {
        case SND_LIGHTS_ON:
          PlaySound(SND_LIGHTS_OFF);
          break;
        case SND_BRIGHT:
        default:
          PlaySound(SND_DARK);
          break;
      }
    }
  }
  else //Normal Mode
  {
    if (lightOn)
    {
      //Random pick from daytime sounds
      PlaySound(random(SND_SUN_IS_UP, SND_WASH_YOUR_HANDS_LUNCHTIME+1));
    } 
    else //afternoon
    {
      //Random pick from afternoon sounds
      PlaySound(random(SND_WASH_YOUR_HANDS_LUNCHTIME, SND_BRUSH_TEETH_BEDTIME+1));
    }
  }
}

void ExecuteBirdsChanged(bool littleBirdUp)
{
  Serial.print("Birds Changed - Little Bird ");
  Serial.println((littleBirdUp ? "Up" : "Down"));
  
  if (OppositeModeSelected)
  {
    if (littleBirdUp)
    {
      PlaySound(SND_BIG_BIRD_DOWN);
    }
    else
    {
      PlaySound(SND_LITTLE_BIRD_DOWN);
    }
  }
}

void ExecuteWindowChanged(bool daytimeView)
{  
  Serial.print("Window Changed: ");
  Serial.println((daytimeView ? "Daytime" : "Nighttime"));
  if (OppositeModeSelected)
  {
    if (daytimeView)
    {
      PlaySound(SND_SUN);  
    }
    else
    {
      PlaySound(SND_MOON);
    }
  }
  else //Normal Mode
  {
    if (daytimeView)
    {
      InMorningMode = !InMorningMode;      
      if(InMorningMode)
      {
        PlaySound(SND_LETS_PRETEND_ITS_MORNING);
      }
      else //afternoon
      {
        PlaySound(SND_LETS_PRETEND_ITS_LUNCHTIME);
      }      
    }
    else
    {
      PlaySound(SND_LETS_PRETEND_ITS_NIGHTTIME);
    }
  }
}

void ExecuteModeChanged(bool oppositeMode)
{  
  Serial.print("Mode Changed: ");
  Serial.println((oppositeMode ? "Opposites" : "Normal"));
  if (oppositeMode)
  {
    PlaySound(SND_OPPOSITES);
  }
  else //Normal Mode
  {
    //When changing back to normal mode, let the current window view
    //control the played message so fake a window changed event
    ExecuteWindowChanged(DaytimeWindowSelected);
  }
}

