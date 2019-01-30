#include <DHT.h>
#define DHTTYPE DHT22

DHT dht(6, DHTTYPE);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
   // Aguarda 2 segundos entre as medicoes
  delay(2000);
  
  // Leitura da umidade
  float h = dht.readHumidity();
  // Leitura da temperatura (Celsius)
  float t = dht.readTemperature();
   Serial.print("Temperatura: "); 
  Serial.print(t);
  Serial.print(" *C  ");
  // Mostra a umidade no serial monitor e no display
  Serial.print("Umidade : "); 
  Serial.print(h);
}
