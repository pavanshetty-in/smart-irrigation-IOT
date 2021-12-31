#include "DHT.h"

#define DHTPIN 2// Digital pin connected to the DHT sensor
#define soil_s A0
#define DHTTYPE DHT11   // DHT 11
float soil_moist; //random variable 

DHT dht(DHTPIN, DHTTYPE);

int motor_on();

void setup() {
  pinMode(3,OUTPUT); 
  pinMode(1,INPUT);
  Serial.begin(9600);
  
  dht.begin();
}

void loop() { 
  soil_moist = analogRead(soil_s);
  soil_moist = map(soil_moist,550,10,0,100);
  Serial.print(soil_moist);
  Serial.println("%");
  if(soil_moist <= 0) 
  {
    motor_on();
  }
  else
  {
    digitalWrite(3,HIGH); 
  }
    Serial.print(dht.readHumidity());
    Serial.println("hum ");
    Serial.print(dht.readTemperature());
    Serial.println("C ");
    Serial.println("******");

  delay(10000); 
}

int motor_on()
{
   digitalWrite(3,LOW); 
   Serial.println("motor on @"); 
}
