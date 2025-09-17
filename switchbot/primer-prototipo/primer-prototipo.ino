/*
  Primera prueba SwitchBot
*/
#include <ESP32Servo.h>

#define btn0   26   // pin del boton para el switch manual
#define SmPin  12   // pin para controlar el servomotor
#define pRed   14
#define pGreen 27

Servo servoMotor;

void setup() {
  Serial.begin(115200);

  pinMode(btn0, INPUT); // configuramos el pin del boton 0 como entrada

  pinMode(pRed, OUTPUT);
  pinMode(pGreen, OUTPUT);
  
  servoMotor.attach(SmPin); // configuramos el pin de control al servo

  // servoMotor.write(0);
  // delay(1000);
  // servoMotor.write(150);
  // delay(1000);
}

bool state;
void loop() {
  int btnValue = digitalRead(btn0);
  Serial.println(btnValue);
  if (btnValue) {
    int pColor = state ? pGreen:pRed;
    digitalWrite(pColor, HIGH);
    servoMotor.write(state ? 0:180);
    state = !state;
    delay(750);
    digitalWrite(pColor, LOW);
  }
  delay(250);
}
