int N = 5;            // numero de leds y rangos
const float sens = 1;
const float ref_voltage = 3.3;
int pinesLed[5] = {
  13, 12, 14, 27, 4
};
int pinLDR = 34;

float rangos[5] = {
  0.5, 0.7, 1.4, 1.8, 1.6
};

void set_range(const float &sens_)
{
  float max_value = 3.3 * sens_;
  for(size_t i = 0; i<N; i++)
  {
    rangos[i] = max_value / N * (i + 1);
    Serial.print(rangos[i]);
    Serial.print(" ");
  }
  Serial.println();
  delay(3000);
}

void set_rangos(float step) {
  float v_inicial = 0.3;
  for (int i = 0; i < N; i++) {
    rangos[i] = v_inicial + (step * i);
    Serial.print(rangos[i]);
    Serial.print(" ");
  }
  Serial.println();
  delay(3000);
}

float ldr_value = 0, voltaje = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  for (int i = 0; i < N; i++)
    pinMode(pinesLed[i], OUTPUT);

  //set_rangos(0.7);
  set_range(0.7);
}

void loop() {
  // put your main code here, to run repeatedly:
  ldr_value = analogRead(pinLDR);
  voltaje = ldr_value * (3.3) / (4095);

  Serial.print("adc value: ");
  Serial.print(ldr_value);
  Serial.print(", voltaje: ");
  Serial.println(voltaje);

  for (int i = 0; i < N; i++) {
    if (voltaje >= rangos[i])
      digitalWrite(pinesLed[i], HIGH);
    else
      digitalWrite(pinesLed[i], LOW);
  }
  delay(250);
}
