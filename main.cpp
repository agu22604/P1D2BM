#include <Arduino.h>
#include "display7seg.h"
#include <stdint.h>
#include <driver/ledc.h>

// Arreglo para almacenar los dígitos
int digitos[4] = {0, 0, 0, 0};  // Array para almacenar los dígitos
int digitoIndex = 0;


// Configuración de pines PWM para el LED RGB
#define LR 4
#define LV 2
#define LA 15
// Configuración PWM
#define CANALR 0
#define CANALV 1
#define CANALA 2
#define FRECUENCIA_PWM 5000 // 5 kHz
#define RESOLUCION_PWM 8   // Resolución de 8 bits (0-255)


// Sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DISP1 18
#define DISP2 5
#define DISP3 19

#define sA 21
#define sB 23
#define sC 14
#define sD 12
#define sE 13
#define sF 22
#define sG 27
#define sP 26

// Botón
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

struct Button {
    const uint8_t PIN;
    volatile uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {33, 0, false};
float resultado;

// Interrupción del botón
void IRAM_ATTR BTN1_ISR() {
    portENTER_CRITICAL_ISR(&mux);
    button1.numberKeyPresses++;
    button1.pressed = true;
    portEXIT_CRITICAL_ISR(&mux);
}

// Función para inicializar PWM
void initPWM() {
    // Configura el canal PWM para cada color
    ledcSetup(CANALR, FRECUENCIA_PWM, RESOLUCION_PWM);
    ledcSetup(CANALV, FRECUENCIA_PWM, RESOLUCION_PWM);
    ledcSetup(CANALA, FRECUENCIA_PWM, RESOLUCION_PWM);

    // Asocia los pines de los LEDs a los canales PWM
    ledcAttachPin(LR, CANALR);
    ledcAttachPin(LV, CANALV);
    ledcAttachPin(LA, CANALA);
}

// Define el pin donde está conectado el DHT11
#define DHTPIN 25

// Define el tipo de sensor que estás usando
#define DHTTYPE DHT11

// Inicializa el sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Función para convertir el número a un array de dígitos
void convertirANumeros(float num) {
  String numStr = String(num, 2); // 2 dígitos decimales
  
  // Limpiar el array de dígitos antes de llenarlo nuevamente
  for (int i = 0; i < 4; i++) {
    digitos[i] = 0;
  }
  
  digitoIndex = 0; // Reiniciar el índice

  // Recorrer cada carácter de la cadena
  for (int i = 0; i < numStr.length(); i++) {
    char c = numStr.charAt(i);  // Obtener el carácter en la posición i
    
    if (c >= '0' && c <= '9') {
      digitos[digitoIndex++] = c - '0';  // Convertir el carácter a entero y almacenarlo en el array
    } else if (c == '.') {
      // Aquí puedes manejar el punto decimal según cómo deseas representarlo
      
    }
  }
}

void setup() {
  configuraDisplay(sA, sB, sC, sD, sE, sF, sG, sP);
  pinMode(DISP1, OUTPUT);
  pinMode(DISP2, OUTPUT);
  pinMode(DISP3, OUTPUT);

  // Inicia la comunicación serie
  Serial.begin(115200);
  // Inicia el sensor DHT
  dht.begin();

  // Configura el pin del botón como entrada con resistencia pull-up
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, BTN1_ISR, FALLING);

  initPWM();
}

void loop() {
  // Activar cada display secuencialmente
  digitalWrite(DISP1, HIGH);
  desplegarDisplay(digitos[0]);
  desplegarPunto(1);
  delay(5);
  digitalWrite(DISP1, LOW);

  digitalWrite(DISP2, HIGH);
  desplegarDisplay(digitos[1]);
  desplegarPunto(0);
  delay(5);
  digitalWrite(DISP2, LOW);

  digitalWrite(DISP3, HIGH);
  desplegarDisplay(digitos[2]);
  desplegarPunto(1);
  delay(5);
  digitalWrite(DISP3, LOW);

  if (button1.pressed == true) {
    // Lee la temperatura en grados Celsius
    float temperatura = dht.readTemperature();

    // Verifica si las lecturas fallaron
    if (isnan(temperatura)) {
      Serial.println("Error al leer el sensor DHT11");
      return;
    }

    // Actualiza el valor de la temperatura
    resultado = temperatura;

    // Convierte la temperatura a dígitos para el display
    convertirANumeros(resultado);

    // Restablece el estado del botón
    button1.pressed = false;
  }

  Serial.print("Temperatura almacenada: ");
  Serial.println(resultado);

  // Ajusta el color según la temperatura
    uint8_t rojo, verde, azul;

    if (resultado < 20) {
        // Frío: Azul
        rojo = 0;
        verde = 0;
        azul = 255;
    } else if (resultado >= 20 && resultado < 30) {
        // Templado: Verde
        rojo = 0;
        verde = 255;
        azul = 0;
    } else {
        // Caliente: Rojo
        rojo = 255;
        verde = 0;
        azul = 0;
    }

    // Ajusta el PWM para cada color
    ledcWrite(CANALR, rojo);
    ledcWrite(CANALV, verde);
    ledcWrite(CANALA, azul);

    
}
