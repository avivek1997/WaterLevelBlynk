#include<Arduino.h>
struct Configs
{
    String IP="";
    int motor_Pin = D1;
    int low_Pin = D5;
    int high_Pin = D6;
    int sump_Pin = A0; //V5
    bool controllerAuto_Manual=1;
    time_t motorStartTime;
    int maxMotorRunTimeInSec=3600;//5600 - 1.5hrs , 3600 - 1 hr
}Configs;
