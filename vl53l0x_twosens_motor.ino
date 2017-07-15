//two sensors and one motor driving testprogram,
//will change direction when distance is less than 90mm
#include <Wire.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
const byte DRV8830 = 0x60; //DRV8830 address
int count=0;
int sensnum;
int direct;
boolean direc=LOW;
byte mot_speed;

void setup() {
//  Serial.begin(115200);
  Wire.begin();
  writeDRV8830(0x00, 0x00);
  // wait until serial port opens for native USB devices
//  while (! Serial) {
//    delay(1);
//  }
  
 // Serial.println("Adafruit VL53L0X two sensors");
  
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  for(int i=11;i<14;i++){
    pinMode(i,OUTPUT);
  }

  direc=HIGH;//initial rotation to forward (back:0)
  mot_speed=33; //initial speed, max:63
  sens_init(direc); //initiazlize sensor #1
}


void loop() {
  int distance;
  writeDRV8830(mot_speed, direc);
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
  distance=measure.RangeMilliMeter;
  
  } else {
  distance=9999;//out of range
  }

  if(distance<90){
    direc=!direc;
    sens_init(direc);   
  }
//  Serial.println(distance);
//  Serial.println(direc);
  LED_indicator(direc);
  //delay(100);
}

void sens_init(boolean sens){
  if(sens==HIGH){
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
//      Serial.println("sens1");
  }else{
    digitalWrite(4,HIGH);
    digitalWrite(5,LOW);    
//    Serial.println("sens2");
  }
  lox.begin();
//  delay(200);
}

void LED_indicator(boolean number){
    if(number==1){
//    sensnum=1;
    digitalWrite(11,LOW);
    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);    
  }else{
    digitalWrite(11,HIGH);
    digitalWrite(12,LOW);
    digitalWrite(13,HIGH);      
}
}

//void motor(int height2, byte param3){
//  byte param2;
////expected maximum height=600,min=100 mm
////param 3 = 0:stand-by, 1:backward, 2:forward, 3:break
////param 2 = minimum 10, maximum 63 speed
//  param2=(height2/100)*12+30;
//  if((param2>62)||(param2==4000)){
//    param2=6;
//    param3=3;
////    leddrive(3);
//  }
//  writeDRV8830(param2,param3);
//  delay(10);
//}

void writeDRV8830(byte vset, byte dir){
  byte vdata,data;

  if(dir==1){
    data=1;
  }else{
    data=2;
  }
  vdata = vset <<2 | data;
  Wire.beginTransmission(DRV8830);
  Wire.write(0x00);
  Wire.write(vdata);
  Wire.endTransmission(true);
  delay(10);
}

