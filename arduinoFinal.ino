#include "TimerOne.h"
#include <PID_v1.h>

const byte motor_interruptA1 = 2;
const byte motor_interruptB1 = 3;
const byte motor_interruptA2 = 18;
const byte motor_interruptB2 = 19;
unsigned long counter1 = 0;
unsigned long counter2 = 0;
long nbCap = 17;
float mesure1 = 0 ;
float mesure2 = 0;
int signe1 = 1;
int signe2 = 1;
int pinDir1 = 34;
int pinPwm1 = 10;
int pinDir2 = 32;
int pinPwm2 = 13;
char master = 'm';
int v1 = 0 ;
int v =0;
int v2 = 0;
int d1 = 0;
int d2 = 0;
const float kp = 0.01413;
const float ki = 2.542782;
const float kd = 0.000008;
int distance = 0 ;
//float erreur1 = 0;
//float erreur_precedente1 = 0 ;
//float somme_erreur1 = 0;
//float diff_erreur1 = 0;
//float erreur2 = 0;
//float erreur_precedente2 = 0 ;
//float somme_erreur2 = 0;
//float diff_erreur2 = 0;
//double Setpoint, Input, Output;
//PID myPID(&Input, &Output, &Setpoint,kp,ki,kd, DIRECT);


void ISR_countA1() {
  counter1++;
  
}
void ISR_countB1() {
  counter1++;
 
  if (digitalRead(motor_interruptA1)) {
    signe1 = 1;
  }
  else {
    signe1 = -1;
  }
}
void ISR_countA2() {
  counter2++;
}
void ISR_countB2 () {
  counter2++;
 
  if (digitalRead(motor_interruptA2)) {
    signe2 = 1;
  }
  else {
    signe2 = -1;
    
  }
}
void ISR_timerone() {
  Timer1.detachInterrupt();
  mesure1 = (float)(counter1 / nbCap * 60.0 * signe1); // Avons-nous vraiment besoin du " (float) " ?
  Serial1.print(mesure1);
  Serial1.print(' ');
  Serial1.print(counter1);
  counter1 = 0;
  Serial1.print(' ');
  mesure2 = (float)(counter2 / nbCap * 60.0 * signe2);
   Serial1.print(mesure2);
  Serial1.print(' ');
  Serial1.println(counter2);
  distance += max(mesure1, mesure2) *6.28/60;
  delay(100);
  counter2 = 0;
  Timer1.attachInterrupt(ISR_timerone);
}
void setup() {
  pinMode ( pinDir1 , OUTPUT);
  pinMode ( pinPwm1 , OUTPUT);
  pinMode ( pinDir2 , OUTPUT);
  pinMode ( pinPwm2 , OUTPUT);
  Serial1.begin(9600);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(ISR_timerone);
  attachInterrupt(digitalPinToInterrupt(motor_interruptA1), ISR_countA1, RISING);
  attachInterrupt(digitalPinToInterrupt(motor_interruptB1), ISR_countB1, RISING);
  attachInterrupt(digitalPinToInterrupt(motor_interruptA2), ISR_countA2, RISING);
  attachInterrupt(digitalPinToInterrupt(motor_interruptB2), ISR_countB2, RISING);
//  Input = map(mesure1,-3300,3300, -255,255);
//  Setpoint = 00;
//  //turn the PID on
//  myPID.SetMode(AUTOMATIC);
}

void loop() {
  if ( Serial1.available () > 0 )
  {
    master = Serial1.read();
    switch (master)
    {
      case ('z') : v1 += 10 ; v2 += 10;
        
        break ;
      case ('s') : v1 -= 10 ; v2 -= 10;
       
        break ;
      case ('b') : v1 = v2 = 0 ;
        
        break;
      case ('d') : v1 += 5; v2 -= 5; if (v1 > 255) {
          v2 -= (v1 - 255);
        } else if (v2 < -255) {
          v1 += (-255 - v2);
        }
        
        break;
      case ('q') : v1 -= 5; v2 += 5 ; if (v1 < -255) {
          v2 += (-255 - v1);
        } else if (v2 > 255) {
          v1 -= (v2 - 255);
        }
        
        break;
      case ('g') : v1 = 255; v2 = -255; break;
      case ('o') : v1 = -255; v2 = 255; break;
      case('a') : v1 = 255 ; v2= 255;break ;
      case('e') : v1 = -255 ; v2= -255;break ;
    }
    v1 = constrain(v1, -255 , 255); v2 = constrain(v2, -255, 255);
   if(distance>= 5) {
    v1 =0;
    v2 = 0;
    distance = 0;
   }
    //v = v1;
    } 

 else {// calcul de pid 
    //     Setpoint = v ;
//    Input = map( mesure1 , -3300 , 3300, -255 ,255) ;
//    myPID.Compute();
//    // the convenient v1 to the driver 
//    v1 = Output ;
    
}
    if (v1 < 0) {
      analogWrite(pinPwm1, -v1);
      d1 = 1;
    }
    else {
      analogWrite(pinPwm1, v1);
      d1 = 0;
    }
    if (v2 < 0) {
      analogWrite(pinPwm2, -v2);
      d2 = 1;
    }
    else {
      analogWrite(pinPwm2, v2);
      d2 = 0;
    }
    if (d1 == 0) {
      digitalWrite(pinDir1, LOW);
    } else {
      digitalWrite(pinDir1, HIGH);
    }
    if (d2 == 0) {
      digitalWrite(pinDir2, LOW);
    } else {
      digitalWrite(pinDir2, HIGH);
    }
    //     Serial1.print(v1);Serial1.println(d1);
    //     Serial1.print(v2);Serial1.println(d2);
    delay(500);
    
  
 }

void pidController ()  {
    //    
    //    erreur1 = map(v1,-255,255,-3300,3300) - mesure1;
    //    diff_erreur1 = erreur1 - erreur_precedente1;
    //    somme_erreur1 += erreur1;
    //    erreur_precedente1 = erreur1;
    //    v1=map( kp * erreur1 + kd * diff_erreur1 + ki * somme_erreur1,-3300,3300,-255,255);
    //    erreur2 = map(v2,-255,255,-3300,3300) - mesure2;
    //    diff_erreur2 = erreur2 - erreur_precedente2;
    //    somme_erreur2 += erreur2;
    //    erreur_precedente2 = erreur2;
    //    v2=map(kp*erreur2 + kd*diff_erreur2 + ki*somme_erreur2,-3300,3300,-255,255);
    //    delay(500);
  }
