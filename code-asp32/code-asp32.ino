#include <Wire.h>

// Motor 1 = Esquerdo
int motor1Pin1 = 27; //input2 - out4
int motor1Pin2 = 26; //input1 - out3
int enable1Pin = 14;

// Motor 2 = Direito
int motor2Pin1 = 15; 
int motor2Pin2 = 2; 
int enable2Pin = 4; 

// Propriedade dos PWM
const int freq = 30000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int resolution = 8;

// Inicia com 60% de velocidade
int dutyCycle = 255; //0 a 255 - 0 = Sem velocidade e 255 = maxima velocidade

//Pinos Sensores
const int s2Pin = 33; //esquerda
const int s3Pin = 32; //central
const int s4Pin = 35; //direita

void setup() {
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
  //Velocidade da porta serial
  Serial.begin(115200);
  //Esta função inicializa a biblioteca Wire e se conecta ao barramento I2C como controlador ou periférico
  Wire.begin();

}

void loop() {
  //Faz a leitura dos sensores
  int s2 = analogRead(s2Pin);
  int s3 = analogRead(s3Pin);
  int s4 = analogRead(s4Pin);
  
  //Logica de Direcionamento
  //esquerda e direita brancos, central preto
  if(s2 > 0 && s3 == 0 && s4 > 0) {
    //Mantem os dois motores ligados na constante dutyCycle sem direcionar
    Serial.printf("\nCentralizado s2 %d, s3 %d, s4 %d",s2,s3,s4);
    ledcWrite(pwmChannel1, dutyCycle);
    ledcWrite(pwmChannel2, dutyCycle);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 

  }
  //central e direita brancos, esquerda preto --> direciona para esquerda
  else if(s2 == 0 && s3 > 0 && s4 > 0){
    //Para direcionar para esquerda, reduz a velocidade do motor esquerdo
    Serial.printf("\nEsquerda s2 %d, s3 %d, s4 %d",s2,s3,s4);
    ledcWrite(pwmChannel1, 0);
    ledcWrite(pwmChannel2, dutyCycle);
  }
  //central e esquerda brancos, direita preto --> direciona para direita
  else if(s2 == 0 && s3 > 0 && s4 == 0){
    //Para direcionar para direita, reduz a velocidade do motor direito
    Serial.printf("\nDireita s2 %d, s3 %d, s4 %d",s2,s3,s4);
    ledcWrite(pwmChannel1, 0);
    ledcWrite(pwmChannel2, dutyCycle);
  }
  // caso contrario, perdemos a linha e desligamos os motores
  else {
    Serial.printf("\nPerdemos a linha s2 %d, s3 %d, s4 %d",s2,s3,s4);
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
  }

}

/*
// Move the DC motor forward at maximum speed
  Serial.println("Moving Forward");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  delay(2000);

  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);

  // Move DC motor backwards at maximum speed
  Serial.println("Moving Backwards");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  delay(2000);

  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);

  // Move DC motor forward with increasing speed
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  while (dutyCycle <= 255){
    ledcWrite(pwmChannel, dutyCycle);   
    Serial.print("Forward with duty cycle: ");
    Serial.println(dutyCycle);
    dutyCycle = dutyCycle + 5;
    delay(500);
  }
  dutyCycle = 200;
*/

