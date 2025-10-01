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
#define pinEna   3 // pin para habilitar la alimentacion del servo
#define pinSm 4 // pin para la señal pwm del servo
#define TIME_TO_SLEEP 5000

/**** OBJETOS A USAR ****/
Servo servoMotor;

/**** VARIABLES GLOBALES ****/
bool state = false; // estado del switchbot, true:on, false:off
const int ANGLE_1 = 0;
const int ANGLE_2 = 180;
int ANGLE_ON  = ANGLE_1;
int ANGLE_OFF = ANGLE_2;
int counter = 0;
bool sleep_mode = false;

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
  pinMode(pinEna, OUTPUT);
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
  // nos aseguramos de que no este en sleep mode
  // en caso contrario, lo desactivamos
  counter = 0;
  if (sleep_mode) {
    digitalWrite(pinEna, HIGH);
    sleep_mode = false;
    delay(200);
  }

  // cambiamos el estado a on o off
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
  digitalWrite(pinEna, HIGH);
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
  if (sleep_mode == false) {
    if (counter <= TIME_TO_SLEEP) {
      counter += 250;
    } else {
      digitalWrite(pinEna, LOW);
      sleep_mode = true;
    }
    Serial.println(counter);
  }
  delay(250);
}
