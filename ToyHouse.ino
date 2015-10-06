#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

//Definitions for sound tracks
//Any track that is a song that should not be interrupted
//should have 100 added to the number.
#define SND_BRIGHT                      1
#define SND_DARK                        2
#define SND_LIGHTS_ON                   3
#define SND_LIGHTS_OFF                  4
#define SND_SUN_IS_UP                   105
#define SND_LETS_PRETEND_ITS_MORNING    6 // LETS-PRETEND_ITS_LUNCHTIME
#define SND_LETS_PRETEND_ITS_NIGHTTIME  7
#define SND_OPPOSITES                   8
#define SND_SUN                         9
#define SND_MOON                        10
#define SND_LITTLE_BIRD_DOWN            11
#define SND_BIG_BIRD_DOWN               12
#define SND_WASH_YOUR_HANDS             13
#define SND_TAKING_A_BATH               14
#define SND_FULL_OF_BUBBLES             15
#define SND_LITTLE_RUBBER_DUCK_SONG     116
//
#define SND_SHORT_DOORBELL              17
#define SND_LONG_DOORBELL               18
#define SND_QUACK_LONG                  19
#define SND_QUACK_SHORT                 20
#define SND_QUACK_LOUD                  21
#define SND_QUACK_QUIET                 22

#define SND_NIGHT                       19
#define SND_DAY                         20
      
//Definiations for house inputs and outputs
//0 and 1 are reserved for the serial port
#define INPUT_LIGHT_SWITCH 11
#define INPUT_DOORBELL     2
#define INPUT_BIRDS        3
#define INPUT_FRIDGE_DOOR  4
#define INPUT_BOOK         5
#define INPUT_DUCK         6
#define INPUT_WINDOW       7
#define INPUT_MODE         8
#define OUTPUT_LAMP        9

#define INPUT_BUSY_SIGNAL  10

bool LightSwitchOn;
enum LightSwitchStates
{
  OP_LightsOn,
  OP_LightsOff,
  OP_Bright,
  OP_Dark,
  NO_SunIsUpSong
} LightSwitchState;

bool DoorbellPressed;
bool LittleBirdDown;
bool FridgeDoorOpen;
bool BookPressed;
bool DuckPressed;
bool DaytimeWindowSelected;
bool OppositeModeSelected;

int DaytimeDuckSounds[4];

bool PlayingSong;

void PlaySound(int soundNumber)
{
  //If playing a song, we ignore other sound requests until the song completes
  if (PlayingSong)
    return;
  
  if (soundNumber >= 100)  
    PlayingSong = true;
  
  //Send track to play to sound module
  mp3_play (soundNumber);
  delay(100); //Wait 100ms before moving on to make sure the sound has started playing and busy signal is high
}

void setup()
{
  //Initialize serial port for sound module
  Serial.begin (9600);
  mp3_set_serial (Serial);      //set Serial for DFPlayer-mini mp3 module 
  delay(1);                     // delay 1ms to set volume
  mp3_set_volume (5);          // value 0~30

  //Read in initial state of all buttons so we don't get changed events on everything
  DuckPressed = digitalRead(INPUT_DUCK);
  DoorbellPressed = digitalRead(INPUT_DOORBELL);
  BookPressed = digitalRead(INPUT_BOOK);
  FridgeDoorOpen = digitalRead(INPUT_FRIDGE_DOOR);
  LightSwitchOn = digitalRead(INPUT_LIGHT_SWITCH);
  LittleBirdDown = digitalRead(INPUT_BIRDS);
  DaytimeWindowSelected = digitalRead(INPUT_WINDOW);
  OppositeModeSelected = digitalRead(INPUT_MODE);
  
  //Make sure lamp output is off
  digitalWrite(OUTPUT_LAMP, false);

  //Load arrays for groups of sounds
  DaytimeDuckSounds[0] = SND_WASH_YOUR_HANDS;
  DaytimeDuckSounds[1] = SND_TAKING_A_BATH;
  DaytimeDuckSounds[2] = SND_FULL_OF_BUBBLES;
  DaytimeDuckSounds[3] = SND_LITTLE_RUBBER_DUCK_SONG;
}

void loop()
{    
  //Check status of sound module to determine when song is finished
  if (digitalRead(INPUT_BUSY_SIGNAL) == LOW)
  {
    //When busy signal drops, set PlayingSong to false
    PlayingSong = false;
  }

  //Check our inputs for any that changed state

  //Rubber Duck
  if (ButtonPressed(INPUT_DUCK, &DuckPressed))
    ExecuteDuckButtonPressed();

  //Doorbell
  if (ButtonPressed(INPUT_DOORBELL, &DoorbellPressed))
    ExecuteDoorbellPressed();

  //Book
  if (ButtonPressed(INPUT_BOOK, &BookPressed))
    ExecuteBookPressed();

  //Refrigerator Door
  if (StateChanged(INPUT_FRIDGE_DOOR, &FridgeDoorOpen))
    ExecuteFridgeDoorChanged(FridgeDoorOpen);

  //Light Switch
  if (StateChanged(INPUT_LIGHT_SWITCH, &LightSwitchOn))
    ExecuteLightSwitchChanged(LightSwitchOn);

  //Birds
  if (StateChanged(INPUT_BIRDS, &LittleBirdDown))
    ExecuteBirdsChanged(LittleBirdDown);

  //Window
  if (StateChanged(INPUT_WINDOW, &DaytimeWindowSelected))
    ExecuteWindowChanged(DaytimeWindowSelected);

  //Mode
  if (StateChanged(INPUT_MODE, &OppositeModeSelected))
    ExecuteModeChanged(OppositeModeSelected);
}

bool ButtonPressed(int buttonInput, bool* lastState)
{
  //Look for falling edge of push buttons
  bool inputState = digitalRead(buttonInput);
  if (inputState != *lastState)
  {
    *lastState = inputState;
    return (!inputState);
  }  
}

bool StateChanged(int switchInput, bool* lastState)
{
  bool inputState = digitalRead(switchInput);
  if (inputState != *lastState)
  {
    *lastState = inputState;
    return true;
  }
  return false;
}

void ExecuteDuckButtonPressed()
{
  if (OppositeModeSelected)
  {
    //Scroll through opposite pairs
  }
  else
  {
    if (DaytimeWindowSelected)
    {
      //Random pick from daytime sounds
      //Example code below assumes 4 total songs
      //so will return a value from 0-3 by calling rand(4)
      int soundIndex = random(4);

      //If we picked a song, call PlaySong to set the flag to not interrupt.
      if (DaytimeDuckSounds[soundIndex] == SND_LITTLE_RUBBER_DUCK_SONG)
      {
        PlaySound(DaytimeDuckSounds[soundIndex]);
      }
      else
      {
        PlaySound(DaytimeDuckSounds[soundIndex]);
      }
    }
    else //nighttime
    {
      //Random pick from nighttime sounds
    }
  }
}

void ExecuteDoorbellPressed()
{
  
}

void ExecuteBookPressed()
{
  
}

void ExecuteFridgeDoorChanged(bool doorOpen)
{

}

void ExecuteLightSwitchChanged(bool lightOn)
{
  if (OppositeModeSelected)
  {
    if (lightOn)
    {
      switch (LightSwitchState)
      {
        case OP_LightsOff:
          LightSwitchState = OP_Bright;
          PlaySound(SND_BRIGHT);
          break;
        case OP_Dark:
        default:
          LightSwitchState = OP_LightsOn;
          PlaySound(SND_LIGHTS_ON);
          break;
      }
    }
    else
    {
      switch (LightSwitchState)
      {
        case OP_LightsOn:
          LightSwitchState = OP_LightsOff;
          PlaySound(SND_LIGHTS_OFF);
          break;
        case OP_Bright:
        default:
          LightSwitchState = OP_Dark;
          PlaySound(SND_DARK);
          break;
      }
    }
  }
  else //Normal Mode
  {
    if (lightOn)
    {
      LightSwitchState = NO_SunIsUpSong;
      PlaySound(SND_SUN_IS_UP);
    }
  }
}

void ExecuteBirdsChanged(bool littleBirdDown)
{
  if (OppositeModeSelected)
  {
    if (littleBirdDown)
    {
      PlaySound(SND_LITTLE_BIRD_DOWN);
    }
    else
    {
      PlaySound(SND_BIG_BIRD_DOWN);
    }
  }
}

void ExecuteWindowChanged(bool daytimeView)
{
  if (OppositeModeSelected)
  {
    if (DaytimeWindowSelected)
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
    if (DaytimeWindowSelected)
    {
      PlaySound(SND_LETS_PRETEND_ITS_MORNING);
    }
    else
    {
      PlaySound(SND_LETS_PRETEND_ITS_NIGHTTIME);
    }
  }
}

void ExecuteModeChanged(bool oppositeMode)
{
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

