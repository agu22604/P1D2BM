#include <Arduino.h>
#include "display7seg.h"

//sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


#define DISP1 18//dude 17
#define DISP2 19// dude 21
#define DISP3 5 //26

#define sA 21 //dudoso 5
#define sB 23 //dud 25
#define sC 14 //14
#define sD 12 //12
#define sE 13 //13
#define sF 22 //dud 19
#define sG 27 //27
#define sP 26 //18
//boton
#define Pausa 34
int BT1;
float resultado;

// Define el pin donde está conectado el DHT11
#define DHTPIN 4 // Puedes cambiar el pin según tu conexión

// Define el tipo de sensor que estás usando
#define DHTTYPE DHT11  // DHT11 o DHT22

// Inicializa el sensor DHT
DHT dht(DHTPIN, DHTTYPE);



void setup() {
  configuraDisplay(sA,sB,sC,sD,sE,sF,sG,sP);
  pinMode(DISP1, OUTPUT);
  pinMode(DISP2, OUTPUT);
  pinMode(DISP3, OUTPUT);

   // Inicia la comunicación serie
  Serial.begin(115200);
  // Inicia el sensor DHT
  dht.begin();

  //boton 
  pinMode(Pausa,INPUT_PULLDOWN);

}

void loop() {
   //boton
   BT1=digitalRead(Pausa);
 
  // Activar cada display secuencialmente
  digitalWrite(DISP1, HIGH);
  delay(5); // Pequeña pausa para mantener el display encendido
  digitalWrite(DISP1, LOW);
  desplegarDisplay(4); //posición 3

  digitalWrite(DISP2, HIGH);
  delay(5);
  digitalWrite(DISP2, LOW);
  desplegarDisplay(6); //posición 2
  desplegarPunto(0);

  digitalWrite(DISP3, HIGH);
  delay(5);
  digitalWrite(DISP3, LOW); 
  desplegarDisplay(9); //posición 1
  
  if (BT1==0){
          // Lee la temperatura en grados Celsius
          float temperatura = dht.readTemperature();

          // Verifica si las lecturas fallaron
          if (isnan(temperatura)) {
            Serial.println("Error al leer el sensor DHT11");
            return;
          }

          // Imprime los valores leídos en el monitor serie
          Serial.print("Temperatura: ");
          Serial.print(temperatura);
          Serial.println(" °C");
          temperatura=resultado;
  }
 Serial.print(resultado);
 
}

