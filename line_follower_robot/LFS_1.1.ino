#include <QTRSensors.h>

// Definição dos pinos dos motores e controle
int pinMotorESQ1 = 5;
int pinMotorESQ2 = 6;
int pinMotorDIR1 = 3;
int pinMotorDIR2 = 11;
int enableD = 9;
int enableE = 10;

// Instanciação do sensor QTR
QTRSensors qtr;
const uint8_t SensorCount = 5;
uint16_t sensorValues[SensorCount];

// Constantes PID
const double Kp = 0.55;
const double Kd = 0.5;
double lastError = 0;

// Limite máximo de velocidade
int max_spd = 120; 

void calibrate() {
  delay(1500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);   

  for (uint16_t i = 0; i < 700; i++) {

    qtr.calibrate();
    Serial.println(i);

  }
  
  digitalWrite(13, LOW);
  delay(3000);
}

void controlarMotores(int leftSpeed, int rightSpeed) {
  // Limitar a velocidade
  leftSpeed = constrain(leftSpeed, 40, max_spd);
  rightSpeed = constrain(rightSpeed, 40, max_spd);

  // Controla os motores
  analogWrite(enableE, leftSpeed);
  analogWrite(enableD, rightSpeed);
}

void setup() {
  Serial.begin(9600);

  pinMode(pinMotorDIR1, OUTPUT);
  pinMode(pinMotorESQ1, OUTPUT);
  pinMode(pinMotorDIR2, OUTPUT);
  pinMode(pinMotorESQ2, OUTPUT);
  pinMode(enableD, OUTPUT);
  pinMode(enableE, OUTPUT);

  // Inicializa motores

  digitalWrite(pinMotorESQ1, HIGH);
  digitalWrite(pinMotorESQ2, LOW);
  digitalWrite(pinMotorDIR1, HIGH);
  digitalWrite(pinMotorDIR2, LOW);

  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A1, A2, A3, A4, A5}, SensorCount);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // Indica modo de calibração
  
  calibrate();

  delay(500);
  Serial.println("pronto");
}

void loop() {
  // Leitura da posição da linha
  int position = qtr.readLineBlack(sensorValues);

  // Calcula o erro
  float error = 2000 - position; // Ajuste conforme a necessidade

  // Cálculo PID
  float ajusto = Kp * error + Kd * (error - lastError);
  
  // Atualiza o erro anterior
  lastError = error;

  // Ajusta as velocidades dos motores com base no PID
  float leftSpeed = max_spd - ajusto;
  float rightSpeed = max_spd + ajusto;

  Serial.println(position);

  
  
  // Controla os motores
  controlarMotores(leftSpeed, rightSpeed);

}
