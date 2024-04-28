#ifndef NPS_POP32_MOTOR_H_
#define NPS_POP32_MOTOR_H_

#define Pwm1Pin PA9
#define Dir1Pin PB15
#define Pwm2Pin PA8
#define Dir2Pin PB14

void Motors(int PowL, int PowR){
  PowL = (PowL * 255) / 100;
  PowR = (PowR * 255) / 100;
  if (PowL >= 0){
    digitalWrite(Dir1Pin, LOW);
    analogWrite(Pwm1Pin, PowL);
  } else {
    digitalWrite(Dir1Pin, HIGH);
    PowL = PowL * (-1);
    analogWrite(Pwm1Pin, PowL);
  }
  if (PowR >= 0){
    digitalWrite(Dir2Pin, LOW);
    analogWrite(Pwm2Pin, PowR);
  } else {
    digitalWrite(Dir2Pin, HIGH);
    PowR = PowR * (-1);
    analogWrite(Pwm2Pin, PowR);
  }
}

#endif
