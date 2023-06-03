#include <Wire.h>
#include "BluetoothSerial.h"

// Motor 1
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 14;

// Motor 2
int motor2Pin1 = 15;
int motor2Pin2 = 2;
int enable2Pin = 4;

// Propriedade dos PWM
const int freq = 50000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int resolution = 8;

// velocidade
int dutyCycle = 178;  //0 a 255 - 0 = Sem velocidade e 255 = maxima velocidade

//Pinos Sensores
const int s2Pin = 33;  //esquerda
const int s3Pin = 32;  //central
const int s4Pin = 35;  //direita

char acionaMotor;

/*Habilitar o Bluetooth*/
//#if!defined(CONFIG_BT_ENABLED) || !define(CONFIG_BLUEROID_ENABLED)
//#error Blueetooth is not enabled!
//#endif

BluetoothSerial serialBT;

void log(int s2, int s3, int s4);

void setup() {
  //Velocidade da porta serial
  Serial.begin(115200);

  //Bluetooth
  serialBT.begin("RoboBolt_UNEMAT");
  Serial.println("RoboBolt já pode ser pareado.");

  // Pinos do motor 1
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  // Pinos do motor 2
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  // Configura os PWMs
  ledcSetup(pwmChannel1, freq, resolution);
  ledcSetup(pwmChannel2, freq, resolution);

  // Anexa o canal de controle dos PWMs
  ledcAttachPin(enable1Pin, pwmChannel1);
  ledcAttachPin(enable2Pin, pwmChannel2);

  //Esta função inicializa a biblioteca Wire e se conecta ao barramento I2C como controlador ou periférico
  Wire.begin();
}

void loop() {
  acionaMotor = (char)serialBT.read();  //Faz a leitura do serial bluetooth
  if (Serial.available()) {
    serialBT.write(Serial.read());
  }

  if (serialBT.available() && acionaMotor == 'L') {  //liga o motor
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    //Quando der a partida, aciona a maxima velocidade
    ledcWrite(pwmChannel1, 255);
    ledcWrite(pwmChannel2, 255);
  } else if (serialBT.available() && acionaMotor == 'D') {  //desliga o motor
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
  }

  //Faz a leitura dos sensores
  int s2 = analogRead(s2Pin);
  int s3 = analogRead(s3Pin);
  int s4 = analogRead(s4Pin);

  //Logica de Direcionamento
  //esquerda e direita brancos, central preto
  if (s2 > 0 && s3 == 0 && s4 > 0) {
    //Mantem os dois motores ligados na constante dutyCycle sem direcionar
    log(s2,s3,s4);
    ledcWrite(pwmChannel1, dutyCycle);
    ledcWrite(pwmChannel2, dutyCycle);
  }
  //esquerda preto --> direciona para esquerda
  else if (s2 == 0) {
    //Para direcionar para esquerda, reduz a velocidade do motor esquerdo
    log(s2,s3,s4);
    ledcWrite(pwmChannel1, 70);
    ledcWrite(pwmChannel2, 255);
  }
  //direita preto --> direciona para direita
  else if (s4 == 0) {
    //Para direcionar para direita, reduz a velocidade do motor direito
    log(s2,s3,s4);
    ledcWrite(pwmChannel1, 255);
    ledcWrite(pwmChannel2, 70);
  } else
    log(s2,s3,s4);
}

void log(int s2, int s3, int s4) {
  Serial.printf("\nLeitura s2 %d, s3 %d, s4 %d, dutyCycle %d", s2, s3, s4, dutyCycle);
}
