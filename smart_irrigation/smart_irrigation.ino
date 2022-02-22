#include "DHT.h"
#include "SoftwareSerial.h"
#include "ArduinoJson.h"

SoftwareSerial myserial(10,11);                //rx 10, tx 11
#define DHTPIN 2                             // Digital pin connected to the DHT sensor
#define soil_s A0
#define DHTTYPE DHT11                        // DHT 11
float soil_moist,temp,humidity; 
int motorstatus,count=0;
char deviceID[10] = "ROYAL256";
//StaticJsonDocument<200> doc;  

DHT dht(DHTPIN, DHTTYPE);

int motor_on();
int send_wethardata();
void ShowSerialData();

void setup() {
  pinMode(3,OUTPUT); 
  pinMode(1,INPUT);
  myserial.begin(9600);
  Serial.begin(9600);
  
  dht.begin();
}

void loop() { 
  soil_moist = analogRead(soil_s);
  soil_moist = map(soil_moist,550,10,0,100);
  temp = dht.readTemperature();
  humidity = dht.readHumidity(); 
  Serial.print(soil_moist);
  Serial.println("%");
  if(soil_moist <= 0) 
  {
    motor_on();
  }
  else
  {
    digitalWrite(3,HIGH);
    motorstatus = 0; 
  }
    Serial.print(humidity);
    Serial.println("hum ");
    Serial.print(temp);
    Serial.println("C ");
    Serial.println("******");
    if(count>=10)
    {
      send_wethardata();
      count=0;
    }
    count+=1;

  delay(60000); 
}

int motor_on()
{
   digitalWrite(3,LOW); 
   motorstatus = 1;
   Serial.println("motor on @"); 
}
int send_wethardata()
{
  if (myserial.available())
  Serial.write(myserial.read());

  myserial.println("AT");
  delay(3000);

  myserial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(6000);
  ShowSerialData();

  myserial.println("AT+SAPBR=3,1,\"APN\",\"WWW\"");//APN
  delay(6000);
  ShowSerialData();
  
  myserial.println("AT+SAPBR=1,1");
  delay(6000);
  ShowSerialData();

  myserial.println("AT+SAPBR=2,1");
  delay(6000);
  ShowSerialData();

  myserial.println("AT+HTTPINIT");
  delay(6000);
  ShowSerialData();
  
  myserial.println("AT+HTTPPARA=\"CID\",1");
  delay(6000);
  ShowSerialData();

  StaticJsonDocument<200> doc;
  doc["temp"] = temp;
  doc["humidity"] = humidity;
  doc["soilmoist"] = soil_moist;
  doc["motorstatus"] = motorstatus;

//  serializeJson(doc, Serial);
  serializeJsonPretty(doc, Serial);
  String sendtoserver;
  serializeJsonPretty(doc, sendtoserver);
  delay(2000);

  myserial.println("AT+HTTPPARA=\"URL\",\"https://rnsit-mca-miniproject.herokuapp.com/se\""); //Server address
  delay(2000);
  ShowSerialData();
  
  myserial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(2000);
  ShowSerialData();

  myserial.println("AT+HTTPDATA=" + String(sendtoserver.length()) + ",100000");
  Serial.println(sendtoserver);
  delay(3000);
  ShowSerialData();

  myserial.println(sendtoserver);
  delay(3000);
  ShowSerialData;

  myserial.println("AT+HTTPACTION=1");
  delay(3000);
  ShowSerialData();

  myserial.println("AT+HTTPREAD");
  delay(3000);
  ShowSerialData();

  myserial.println("AT+HTTPTERM");
  delay(4000);
  ShowSerialData;
  
}
void ShowSerialData()
{
  while (myserial.available() != 0)
    Serial.write(myserial.read());
  delay(1000);
 
}
