#include <POP32.h>
#include "NPS_POP32_Motor.h"
#include "NPS_POP32_PID.h"

#ifndef NPS_POP32_H_
#define NPS_POP32_H_

void POP32_Setup(){
  Serial.begin(9600);
  pinMode(Pwm1Pin, OUTPUT);
  pinMode(Dir1Pin, OUTPUT);
  pinMode(Pwm2Pin, OUTPUT);
  pinMode(Dir2Pin, OUTPUT);
  adc.begin(A0, A2, A1, A3);
  calAvg();
}

void SW_Press(){
  ReadSensor();
}

void SW_MCP_Press(){
  ReadMCP3008();
}

#endif
