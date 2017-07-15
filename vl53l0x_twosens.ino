///two sensors driving test program//
#include <Wire.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
const byte DRV8830 = 0x60; //DRV8830 address
int count=0;
int sensnum;
boolean fst,snd;
byte direct;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  writeDRV8830(0x00, 0x00);
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X two sensors");
  
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  for(int i=11;i<14;i++){
    pinMode(i,OUTPUT);
  }
  digitalWrite(5,LOW);//sensor #1 activate
  digitalWrite(4,HIGH);//sensor #2 deactivate
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X sensor#1"));
    while(1);
  }

  digitalWrite(5,HIGH);//sensor #1 activate
  digitalWrite(4,LOW);//sensor #2 deactivate
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X sensor#2"));
    while(1);
  }
  // power 
  fst=LOW;
  snd=HIGH;
  sensnum=2;
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
  direct=2;//initial rotation to forward
}


void loop() {
  //motor(40,direct);
  VL53L0X_RangingMeasurementData_t measure;

  count++;
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  if(count==1){
    count=0;
    fst=!fst;
    snd=!snd;
  digitalWrite(4,fst);//sensor #1 setting
  digitalWrite(5,snd);//sensor #2 setting
  lox.begin();
  LED_indicator(fst);
    
  }
  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Sensor No.");
    Serial.println(sensnum);
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
    
  //delay(50);
}

void LED_indicator(boolean number){
    if(number==1){
    sensnum=1;
    digitalWrite(11,LOW);
    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);    
  }else if(number==2){
    sensnum=2;
    digitalWrite(11,HIGH);
    digitalWrite(12,LOW);
    digitalWrite(13,HIGH);      
  }else{
    digitalWrite(11,HIGH);
    digitalWrite(12,HIGH);
    digitalWrite(13,LOW);      
  }
  
}

void motor(int height2, byte param3){
  byte param2;
//expected maximum height=600,min=100 mm
//param 3 = 0:stand-by, 1:backward, 2:forward, 3:break
//param 2 = minimum 10, maximum 63
  param2=(height2/100)*12+30;
  if((param2>62)||(param2==4000)){
    param2=6;
    param3=3;
//    leddrive(3);
  }
  writeDRV8830(param2,param3);
  delay(10);
}

void writeDRV8830(byte vset, byte data){
  int vdata = vset <<2 | data;
  Wire.beginTransmission(DRV8830);
  Wire.write(0x00);
  Wire.write(vdata);
  Wire.endTransmission(true);
}

