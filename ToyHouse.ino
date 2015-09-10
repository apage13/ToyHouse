//Definitions for sound tracks
#define SND_BRIGHT                      1
#define SND_DARK                        2
#define SND_LIGHTS_ON                   3
#define SND_LIGHTS_OFF                  4
#define SND_SUN_IS_UP                   5
#define SND_LETS_PRETEND_ITS_MORNING    6
#define SND_LETS_PRETEND_ITS_NIGHTTIME  7
#define SND_OPPOSITES                   8
#define SND_SUN                         9
#define SND_MOON                        10
#define SND_LITTLE_BIRD_DOWN            11
#define SND_BIG_BIRD_DOWN               12
      
//Definiations for house inputs and outputs
#define INPUT_LIGHT_SWITCH 1
#define INPUT_DOORBELL     2
#define INPUT_BIRDS        3
#define INPUT_FRIDGE_DOOR  4
#define INPUT_BOOK         5
#define INPUT_DUCK         6
#define INPUT_WINDOW       7
#define INPUT_MODE         8
#define OUTPUT_LAMP        9

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

bool PlayingSong;

void PlaySong(int songNumber)
{
  //Send track to play to sound module
  //Set flag that we are playing a song so don't interrupt
  PlayingSong = true;
}

void PlaySound(int soundNumber)
{
  //Send track to play to sound module
}

void setup()
{
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
}

void loop()
{    
  //If playing a song, we ignore other buttton presses until the song completes
  if (PlayingSong)
  {
    //Check status of sound module to determine when song is finished
    //When finished, set PlayingSong to false
  }
  else
  {
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
    }
    else
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
      PlaySong(SND_SUN_IS_UP);
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
    ExecuteWindowChanged(DaytimeWindowSelected);
  }
}

