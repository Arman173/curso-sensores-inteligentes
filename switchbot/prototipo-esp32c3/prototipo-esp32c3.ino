/*
  SWITCHBOT CON EL ESP32C3 SUPERMINI
  version: 0.0.1
*/
/**** BIBLIOTECAS ****/
#include <ESP32Servo.h>

/**** DEFINICION DE PINES ****/
#define btn0  5 // pin del boton
#define pinR  6 // pin del led rojo
#define pinG  7 // pin del led verde
#define pinB  8 // pin del led azul
#define pinSm 4 // pin para la señal pwm del servo

/**** OBJETOS A USAR ****/
Servo servoMotor;

/**** VARIABLES GLOBALES ****/
bool state = false; // estado del switchbot, true:on, false:off
const int ANGLE_1 = 0;
const int ANGLE_2 = 180;
int ANGLE_ON  = ANGLE_1;
int ANGLE_OFF = ANGLE_2;
/**** FUNCIONES A USAR ****/
void parpadeo(int pin, int times = 3, int millis = 500) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(millis);
    digitalWrite(pin, LOW);
    delay(millis);
  }
}
void configurarSwitchbot() {
  // configuramos los pines de entrada y salida
  pinMode(btn0, INPUT);  // el boton es una entrada
  pinMode(pinR, OUTPUT); // los led son salidas
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  servoMotor.attach(pinSm); // pinSm como salida pwm para el servo
}
void cambiarOrientacion() {
  Serial.println("Cambiando Orientacion...");
  if (ANGLE_ON == ANGLE_1) {
    ANGLE_ON  = ANGLE_2;
    ANGLE_OFF = ANGLE_1;
  } else {
    ANGLE_ON  = ANGLE_1;
    ANGLE_OFF = ANGLE_2;
  }
  parpadeo(pinB, 3, 250);
}
void cambiarEstado(bool new_state) {
  state = new_state;
  int led, angle;
  if (state) {        // true - prender
    led = pinG;
    angle = ANGLE_ON;
    Serial.println("prendiendo...");
  } else {            // false - apagar
    led = pinR;
    angle = ANGLE_OFF;
    Serial.println("apagando...");
  }
  digitalWrite(led, HIGH);
  servoMotor.write(angle);
  delay(1500);
  digitalWrite(led, LOW);
}
// funcion para saber si se mantiene presionado por x tiempo el boton
bool es_presionado(int wait_time) {
  int counter = 0;
  while (digitalRead(btn0)) {
    if (counter >= wait_time) {
      return true;
    }
    counter += 20;
    delay(20);
  }
  return false;
}

/**** FUNCIONES PRINCIPALES ****/
void setup() {
  Serial.begin(115200);
  delay(500);
  configurarSwitchbot();
}
void loop() {
  if (digitalRead(btn0)) {
    if (es_presionado(3000)) {
      // cambiamos orientacion
      cambiarOrientacion();
    } else {
      // cambiamos el estado
      cambiarEstado(!state);
    }
  }
  delay(250);
}
