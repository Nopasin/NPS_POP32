#include <Adafruit_MCP3008.h>

Adafruit_MCP3008 adc;

#ifndef	NPS_POP32_MCP3008_H_
#define	NPS_POP32_MCP3008_H_

extern const int NumSensors;
extern int Pins[], Min[], Max[], Background, last_value;
extern long F[], Val[];
extern int Error, Last_Error, LeftMotor, RightMotor, PowerMotor, Position;

void calAvg(){
  for(int i=0; i<NumSensors; i++){
    Val[i] = (Max[i] + Min[i]) / 2;
  }
}

void readPrivateMCP3008(long *sensor_values){
	unsigned char i;
	for (i = 0; i < NumSensors; i++) sensor_values[i] = adc.readADC(i);
}

void ReadMCP3008(long *sensorValue) {
	unsigned char i;
	readPrivateMCP3008(sensorValue);
	for (i = 0; i < NumSensors; i++) {
	sensorValue[i] = map(sensorValue[i] , Min[i], Max[i] , 1000, 0);
	if (sensorValue[i] > 1000) sensorValue[i] = 1000;
	else if (sensorValue[i] < 0) sensorValue[i] = 0;
	}
}

int ReadLineMCP3008(long *sensorValue) {
  int on_line = 0;
  unsigned long avg = 0;
  unsigned int sum = 0;
  unsigned long value = 0;
  on_line = 0;
  ReadMCP3008(sensorValue);
  avg = 0;
  sum = 0;
  for (int i = 0; i < NumSensors; i++) {
    value = sensorValue[i];
    if (Background == 1) {
      value = 1000 - value;
      sensorValue[i] = value;
    }
    if (value > 200) on_line = 1;
    if (value > 50) {
      avg += value * (i * 1000);
      sum += value;
    }
  }
  if (on_line == 0) {
    if (last_value < ((NumSensors - 1) * 1000) / 2) {
      return 0;
    }
    else {
      return (NumSensors - 1) * 1000;
    }
  }
  else {
    last_value = avg / sum;
  }
  return last_value;
}

void PidMCP(int BaseSpeed, float Kp, float Kd) {
  Position = ReadLineMCP3008(F);
  Error = Position - 3500;
  PowerMotor = (Kp * Error) + (Kd * (Error - Last_Error));
  Last_Error = Error;
  //	LeftMotor = BaseSpeed + PowerMotor;
  LeftMotor = (BaseSpeed + 4) + PowerMotor;
  RightMotor = BaseSpeed - PowerMotor;
  LeftMotor = constrain(LeftMotor, -100, 100);
  RightMotor = constrain(RightMotor, -100, 100);
  Motors(LeftMotor, RightMotor);
}

void PidCurveMCP(int BaseSpeed, float Kp, float Kd) {
  Position = ReadLineMCP3008(F);
  Error = Position - 3500;
  PowerMotor = (Kp * Error) + (Kd * (Error - Last_Error));
  Last_Error = Error;
  //	LeftMotor = BaseSpeed + PowerMotor;
  LeftMotor = (BaseSpeed + 4) + PowerMotor;
  RightMotor = BaseSpeed - PowerMotor;
  if (LeftMotor >= 100) LeftMotor = 100;
  if (LeftMotor <= 40) LeftMotor = -30;
  if (RightMotor >= 100) RightMotor = 100;
  if (RightMotor <= 40) RightMotor = -30;
  Motors(LeftMotor, RightMotor);
}

void ReadMCP3008() {
  while (1) {
    readPrivateMCP3008(F);
    for (int i = 0; i < NumSensors; i++) {
	  oled.text(i, 5, "F[%d] = %d   ", i, F[i]);
    }
    oled.show();
    if(SW_OK()) break;
  }
  beep(); delay(100); beep(); delay(300);
}

//	------------------------------------------------------------------------------------------------------------------- //

void readPrivate(long *sensor_values){
	unsigned char i;
	for (i = 0; i < NumSensors; i++) sensor_values[i] = analog10(Pins[i]);
}

void Read(long *sensorValue) {
	unsigned char i;
	readPrivate(sensorValue);
	for (i = 0; i < NumSensors; i++) {
	sensorValue[i] = map(sensorValue[i] , Max[i], Min[i] , 1000, 0);
	if (sensorValue[i] > 1000) sensorValue[i] = 1000;
	else if (sensorValue[i] < 0) sensorValue[i] = 0;
	}
}

int ReadLine(long *sensorValue) {
  int on_line = 0;
  unsigned long avg = 0;
  unsigned int sum = 0;
  unsigned long value = 0;
  on_line = 0;
  Read(sensorValue);
  avg = 0;
  sum = 0;
  for (int i = 0; i < NumSensors; i++) {
    value = sensorValue[i];
    if (Background == 1) {
      value = 1000 - value;
      sensorValue[i] = value;
    }
    if (value > 200) on_line = 1;
    if (value > 50) {
      avg += value * (i * 1000);
      sum += value;
    }
  }
  if (on_line == 0) {
    if (last_value < ((NumSensors - 1) * 1000) / 2) return 0;
    else return (NumSensors - 1) * 1000;
  }
  else last_value = avg / sum;
  return last_value;
}

int Center_Error = ((NumSensors - 1) * 1000) / 2;
void Pid(int BaseSpeed, float Kp, float Kd) {
  Position = ReadLine(F);
  Error = Position - Center_Error;
  PowerMotor = (Kp * Error) + (Kd * (Error - Last_Error));
  Last_Error = Error;
  LeftMotor = BaseSpeed + PowerMotor;
  RightMotor = BaseSpeed - PowerMotor;
  LeftMotor = constrain(LeftMotor, -100, 100);
  RightMotor = constrain(RightMotor, -100, 100);
  Motors(LeftMotor, RightMotor);
}

void ReadSensor(){
  while (true){
	readPrivate(F);
    for(int i=0; i < NumSensors; i++){
      oled.text(i, 5, "F[%d] = %d   ", i, F[i]);
    }
    oled.show();
    if(SW_OK()) break;
  }
  beep(); delay(100); beep(); delay(300);
}
#endif
