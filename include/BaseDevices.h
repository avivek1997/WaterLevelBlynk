class OutputDevice
{
private:
  int _pin;
  bool _isInverted = false;
  String _deviceName = "";
  String _reason="";
public:
  bool currentState = false;
  long deviceONtime = 0;
  OutputDevice(int pin, String deviceName, bool isInverted = false)
  {
    _pin = pin;
    _isInverted = isInverted;
    _deviceName = deviceName;
    pinMode(_pin, OUTPUT);
    SetOff();
  }
  void Run()
  {
    if(currentState) deviceONtime++;
    else deviceONtime=0;
  }
  void SetOn(String reason="")
  {
    _reason = reason;
    if(reason !="") Serial.println("\n Device "+_deviceName+" Turned ON Due to "+reason);
    _isInverted ? digitalWrite(_pin, LOW) : digitalWrite(_pin, HIGH);
    currentState = true;
  }
  String Reason()
  {
    return _reason;
  }
  void SetOff(String reason="")
  {
    _reason = reason;
    if(reason !="") Serial.println("\n Device "+_deviceName+" Turned OFF Due to "+reason);
    _isInverted ? digitalWrite(_pin, HIGH) : digitalWrite(_pin, LOW);
    currentState = false;
  }
};
class InputDevice
{
private:
  int _pin;
  bool _isNC = false;
  bool _isAnalogPin = false;

public:
  bool currentState = false;
  InputDevice(int pin, bool isNC = false,bool isAnalogPin = false)
  {
    _pin = pin;
    _isNC = isNC;
    _isAnalogPin = isAnalogPin;
    if(!_isAnalogPin)
      pinMode(_pin, INPUT_PULLUP);
    else
      pinMode(_pin,INPUT);
    //digitalWrite(_pin, HIGH);
  }
  bool Read()
  {
    //Serial.println(String(_pin)+" : "+String(digitalRead(_pin)));
    if(!_isAnalogPin)
      currentState = _isNC ? digitalRead(_pin) : !digitalRead(_pin);
    else
      currentState = _isNC ? (analogRead(_pin) > 500) : !(analogRead(_pin) > 500);
    return currentState;
  }
};