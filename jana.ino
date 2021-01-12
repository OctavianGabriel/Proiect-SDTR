#include "DHT.h"
float h;
float t;
#define DHTPIN 16 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
xSemaphoreHandle xMutex=NULL;



void readSensor( void * parameter )
{
/* loop forever */
for(;;)
{
xSemaphoreTake(xMutex, portMAX_DELAY);
h = dht.readHumidity();
t = dht.readTemperature();
vTaskDelay(3000);
xSemaphoreGive(xMutex);

}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
}

void updateFanState( void * parameter )
{
/* loop forever */
for(;;)
{
xSemaphoreTake(xMutex, portMAX_DELAY);
if(t > 25)
{
  digitalWrite(17,LOW);
}
else
{
  digitalWrite(17,HIGH);
}
vTaskDelay(200);
xSemaphoreGive(xMutex);

}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
}

void printData( void * parameter )
{
/* loop forever */
for(;;)
{
xSemaphoreTake(xMutex, portMAX_DELAY);
 Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
vTaskDelay(5000);
xSemaphoreGive(xMutex);

}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
}


void setup() {
   Serial.begin(115200);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  // put your setup code here, to run once:
pinMode(17, OUTPUT);
delay(2000);
xMutex=xSemaphoreCreateMutex();  //xMutex will be storing NULL when Mutex not created
if(xMutex!=NULL) {
  xTaskCreate(
readSensor, /* Task function. */
"readSensor", /* name of task. */
10000, /* Stack size of task */
NULL, /* parameter of the task */
1, /* priority of the task */
NULL); /* Task handle to keep track of created task */

  xTaskCreate(
updateFanState, /* Task function. */
"updateFanState", /* name of task. */
10000, /* Stack size of task */
NULL, /* parameter of the task */
1, /* priority of the task */
NULL); /* Task handle to keep track of created task */

  xTaskCreate(
printData, /* Task function. */
"printData", /* name of task. */
10000, /* Stack size of task */
NULL, /* parameter of the task */
1, /* priority of the task */
NULL); /* Task handle to keep track of created task */
}
}

void loop() {
}
