#define DEBOUNCE_TIME    100

class SwitchMonitor
{
  private:
  int InputNumber;
  unsigned long LastDebounceTime;
  bool LastRead;

  public:
  bool On;

  void Init(int inputNumber)
  {
    InputNumber = inputNumber;
    
    //Enable the pull-up resistors on all the inputs
    pinMode(inputNumber, INPUT_PULLUP);

    //Record the current state for button pressed monitoring
    On = (digitalRead(inputNumber) == LOW);
    LastRead = On;
  }

  bool StateChanged()
  {
    bool inputState = (digitalRead(InputNumber) == LOW);
    if (inputState != LastRead)
    {
      LastDebounceTime = millis();
      LastRead = inputState;
    }
  
    if (((millis() - LastDebounceTime) > DEBOUNCE_TIME) && inputState != On)
    {
      On = inputState;
      return true;
    }
    return false;
  }
};
