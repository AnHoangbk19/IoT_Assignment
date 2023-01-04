
#include "config.h"
#include "DHT.h"
#define DHT11PIN D4 
#define DHTTYPE DHT11
uint8_t Relay = D7;
uint8_t Relay_LED = D6;

AdafruitIO_Feed *led = io.feed("led");
AdafruitIO_Feed *pump = io.feed("pump");
AdafruitIO_Feed *nhietdo = io.feed("nhietdo");
AdafruitIO_Feed *doamdat = io.feed("doamdat");
AdafruitIO_Feed *doamkk = io.feed("doamkk");
DHT DHT(DHT11PIN, DHTTYPE);

void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->toInt());
  if(data->toInt() == 1){
    digitalWrite(Relay_LED, HIGH);
    Serial.println("LED: ON");
  }
  else if (data->toInt() == 0) {
    digitalWrite(Relay_LED, LOW);
    Serial.println("LED: OFF");
  }
  else if (data->toInt() == 2) {
    digitalWrite(Relay, LOW);
    Serial.println("PUMP: OFF");
  }
  else if (data->toInt() == 3) {
    digitalWrite(Relay, HIGH);
    Serial.println("PUMP: ON");
  }

}

void setup() {
  DHT.begin();
  pinMode (2, INPUT);
  pinMode(A0, INPUT);
  pinMode (13, OUTPUT);
  // pinMode (5, OUTPUT);
  digitalWrite(Relay, LOW);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay_LED, LOW);
  pinMode(Relay_LED, OUTPUT);
  // start the serial connection
  Serial.begin(9600);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  led->onMessage(handleMessage);
  pump->onMessage(handleMessage);
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  led->get();
  pump->get();
}

void loop() {

  io.run();
  // pump->onMessage(handleMessage);
  float h = DHT.readHumidity();
  float t = DHT.readTemperature();
  int value = analogRead(A0);
  int blynkVal = map(value,1023,0,0,100);
  Serial.print("Nhiet do: ");
  Serial.println(t);               
  Serial.print("Do am: ");
  Serial.println(h);  
  Serial.print("Do am dat: ");
  Serial.println(blynkVal); 
  Serial.println();                  
  nhietdo->save(t);
  doamkk->save(h);
  doamdat->save(blynkVal);
  delay(10000);
  
}


