#include <DHT.h>
#define Type DHT11
 
intsensePin=8;  
DHT HT(sensePin,Type);
 
float hum;
float temp;
float td;
//===========================================
intLDRbox = A5; 
intLDRopen = A4 ;
floatReadingBox ;
floatReadingOpen ;
int foggy; // fog not present 
 
intdt=3000;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);     
  HT.begin();
  delay(dt);
  Serial.println("Fog forms when the difference between air Temperature and dew point is less than 2.5 ° C");
  pinMode(LDRbox, INPUT); 
  pinMode(LDRopen, INPUT); 
}
 
void loop() {
 
  hum=HT.readHumidity();
  temp=HT.readTemperature();
  td = temp-( (100.-hum)/5. );
 
  Serial.print("Humidity : ");
  Serial.print(hum);
  Serial.print(", Temperature : ");
  Serial.println(temp);
  Serial.print("Dew point : ");
  Serial.println(td);
 
if(temp-td>2.5)
{
Serial.println("No Fog present");
}
else
{
Serial.println("Possibility of Fog Formation"); 
}
  
  Serial.println();
 
//============================================================================================
 
  foggy = 0;
  
  ReadingBox = analogRead(LDRbox);
  ReadingOpen = analogRead(LDRopen);
 
  Serial.print("Normal Environment : "); 
  Serial.print(ReadingBox); 
 
  Serial.print(" , Foggy Environment : "); 
  Serial.println(ReadingOpen); 
 
  if(ReadingBox-ReadingOpen> 3 || ReadingOpen-ReadingBox> 3)
  {
    foggy = 1;
  }
 
  if(foggy == 0)
  {
     Serial.println("Visibilty is not affected "); 
  }
 
  if(foggy == 1)
  {
     Serial.println("Visibilty Is affected "); 
  }
 
  Serial.println(); 
  Serial.println("========================================================================");
  delay(dt);
}
