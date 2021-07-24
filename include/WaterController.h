 
 #include <BaseDevices.h>
OutputDevice motor(Configs.motor_Pin, "Motor", 0);
InputDevice lowLevelSensor(Configs.low_Pin,1);
InputDevice highLevelSensor(Configs.high_Pin,1);
InputDevice sumpLevelSensor(Configs.sump_Pin,1,1);

tmElements_t ConvertSecToTmElements(int totalSeconds)
{
  tmElements_t t = tmElements_t();
  t.Second = (totalSeconds % 60);
  t.Minute = (totalSeconds % 3600) / 60;
  t.Hour = (totalSeconds % 86400) / 3600;
  t.Day = (totalSeconds % (86400 * 30)) / 86400;
  return t;
}
void LogDeviceStatus()
{
 
String s = "";
  s = "Up HIGH:";
  s += highLevelSensor.currentState;
  s += "  UP LOW:";
  s += lowLevelSensor.currentState;
  s += "  sump :";
  s += sumpLevelSensor.currentState;
  s += "  Motor:";
  s += motor.currentState;
  s += "   Auto/manual:";
  s += Configs.controllerAuto_Manual;
  s += "   IP:";
  s += Configs.IP;
  if(motor.currentState)
  {
    tmElements_t motorRunTime = ConvertSecToTmElements(difftime(now(),Configs.motorStartTime));
    String runTime ="";
    s += "   MotorRunTime:";
    runTime= String(motorRunTime.Hour)+ ":"+ String(motorRunTime.Minute)+ ":" + String(motorRunTime.Second);
    s +=runTime;
    Blynk.virtualWrite(V6,runTime);
  }
  else
  {
    Blynk.virtualWrite(V6,motor.Reason());
  }
  Serial.print(s);
  Serial.println();
  Blynk.virtualWrite(V1,s);
  Blynk.virtualWrite(V4,(highLevelSensor.currentState + lowLevelSensor.currentState));
  sumpLevelSensor.currentState ? sumpLed.setColor("#23C48E") : sumpLed.setColor("#D3435C");
}
void SendNotification()
{
  Blynk.virtualWrite(2, motor.currentState);
    String motorMessage = "WaterLevel Alert - Motor Turned ";
    motorMessage = motorMessage + (motor.currentState ? "ON" : "OFF") +" due to " + motor.Reason();
    Blynk.notify(motorMessage);
    if(motor.currentState)
      Configs.motorStartTime=now();
}
void HandleOverRun()
{
  if(!motor.currentState) return;
  int runTimeInSec = difftime(now(),Configs.motorStartTime);
  if(runTimeInSec > Configs.maxMotorRunTimeInSec)
  {
    motor.SetOff("Over Run");
    Blynk.virtualWrite(2, motor.currentState);
    Blynk.virtualWrite(V3,1); //Setting to ManulaMode
  }

}
void HandleSensor()
{
  LogDeviceStatus();
  lowLevelSensor.Read();
  highLevelSensor.Read();
  sumpLevelSensor.Read();
  HandleOverRun();
  if (Configs.controllerAuto_Manual == HIGH) return;
  bool previousMotorState = motor.currentState;
  if (sumpLevelSensor.currentState == HIGH)
  {

    if (lowLevelSensor.currentState == HIGH && highLevelSensor.currentState == HIGH)
    {
      motor.SetOff("Tank Full");
    }
    else if (lowLevelSensor.currentState == LOW && highLevelSensor.currentState == LOW)
    {
      motor.SetOn("Tank Empty");
    }
  }
  else
  {
    motor.SetOff("Sump Empty");
  }
  if( previousMotorState != motor.currentState ) 
  {
    SendNotification();
  }
}
BLYNK_WRITE(V2)
{
  int state=param.asInt();
  state ? motor.SetOn("Manual Trigger") : motor.SetOff("Manual Trigger");
  SendNotification();
}
BLYNK_WRITE(V3)
{
  int state=param.asInt();
  if(state==1) Configs.controllerAuto_Manual=1;
  else if(state==0) Configs.controllerAuto_Manual=0;
}

