#include "PinChangeInterrupt.h"
#include <GyverNTC.h>

#define pin_5V_ENA  2
#define pin_N5V_ENA 3
#define pin_8V_ENA  5
#define pin_N8V_ENA  8
#define pin_switch  0
#define pin_LED     1
#define pin_buzzer  7
#define pin_8V_temp A0
#define pin_5V_temp A1

int flag_ready = 1;
float temp_5v_ch = 25;
float temp_8v_ch = 25;
float threshold = 50;
GyverNTC therm_8V(pin_8V_temp, 12700, 3950);
GyverNTC therm_5V(pin_5V_temp, 12700, 3950);

void setup() {
  // put your setup code here, to run once:
  pinMode(pin_switch, INPUT); //interrupt pin
  
  pinMode(pin_LED, OUTPUT);
  pinMode(pin_LED, OUTPUT);
  pinMode(pin_5V_ENA, OUTPUT);
  pinMode(pin_N5V_ENA, OUTPUT);
  pinMode(pin_8V_ENA, OUTPUT);
  pinMode(pin_N8V_ENA, OUTPUT);
  pinMode(pin_buzzer,OUTPUT);

 attachPCINT(digitalPinToPCINT(pin_switch), switch_chek, CHANGE);
}

void switch_chek (void){
  if(digitalRead(pin_switch) == HIGH){
    digitalWrite(pin_5V_ENA, HIGH);
    digitalWrite(pin_N5V_ENA, HIGH);
    for (volatile long int i = 0; i < 100; i++){}//delay beetwen 5v and 8v enable
    digitalWrite(pin_8V_ENA, HIGH);
    digitalWrite(pin_N8V_ENA, HIGH);
    flag_ready = 0;        
  }
  if(digitalRead(pin_switch) == LOW){
    digitalWrite(pin_5V_ENA, LOW);
    digitalWrite(pin_N5V_ENA, LOW);
    digitalWrite(pin_8V_ENA, LOW);
    digitalWrite(pin_N8V_ENA, LOW);     
    flag_ready = 1;    
  }
}


void loop() {
  //switch_chek();
  // put your main code here, to run repeatedly:
  switch(flag_ready){
    case 0:{
      digitalWrite(pin_LED, HIGH);
      while(!flag_ready){
        temp_8v_ch = therm_8V.getTempAverage();
        temp_5v_ch = therm_5V.getTempAverage();
        if ((temp_8v_ch > threshold)||(temp_5v_ch > threshold)){
          Buzzer_On(2000);
          delay(300);
          Buzzer_Off();
          delay(700);
        }
      }
      }break;
    case 1:{Led_Blink(500);}break;
    default: break;
  }

}

void Buzzer_On(int freq){
  tone(pin_buzzer, freq);
}
void Buzzer_Off(void){
  noTone(pin_buzzer);
}
void Led_Blink(int tim){
  digitalWrite(pin_LED, !digitalRead(pin_LED));
  delay(tim);
}
