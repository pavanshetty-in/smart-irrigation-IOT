#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11
int water,temp, temp1; //random variable 
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  pinMode(3,OUTPUT); //output pin for relay board, this will sent signal to the relay
  pinMode(6,INPUT); //input pin coming from soil sensor
  Serial.begin(9600);
  dht.begin();
}

void loop() { 
  water = digitalRead(6);
   temp=analogRead(A0);// reading the coming signal from the soil sensor
  if(water == HIGH) // if water level is full then cut the relay 
  {
  digitalWrite(3,LOW); // low is to cut the relay
  printf("%s",water);
  
  }
  else
  {
  digitalWrite(3,HIGH); //high to continue proving signal and water supply
  }
//  Serial.println(water);
//  Serial.println(digitalRead(8));

 temp1=temp*0.48828125;
    Serial.println(dht.readHumidity());
    Serial.println(dht.readTemperature());
  delay(1000); 
}
