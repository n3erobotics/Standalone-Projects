//  TOP SPEED MÁXIMA = 125 MOTORES DE 6V
// FALTA BRINCAR COM O PID E METER OS FIOS DOS DOIS MOTORES DO MESMO TAMANHO



#define N_READINGS 5

#define WEIGHT_1 -2
#define WEIGHT_2 -1
#define WEIGHT_3  0
#define WEIGHT_4  1
#define WEIGHT_5  2

int IR1 = A1;
int IR2 = A2;
int IR3 = A3;
int IR4 = A4;
int IR5 = A5;

int P = 0;
int D = 0;
int lasterror = 0;

int centro = 0;


int pinMotorESQ1 = 5;
int pinMotorESQ2 = 6;
int pinMotorDIR1 = 3;
int pinMotorDIR2 = 11;

int enableD = 9;
int enableE = 10;


void setup() {
  Serial.begin(9600);
  pinMode(pinMotorDIR1, OUTPUT);
  pinMode(pinMotorESQ1, OUTPUT);
  pinMode(pinMotorDIR2, OUTPUT);
  pinMode(pinMotorESQ2, OUTPUT);
  pinMode(enableD, OUTPUT);
  pinMode(enableE, OUTPUT);

  calibrate();
  delay(1000);
  analogWrite(enableD, 80);
  analogWrite(enableE, 80);

  digitalWrite(pinMotorESQ1, HIGH);
  digitalWrite(pinMotorESQ2, LOW);
  digitalWrite(pinMotorDIR1, HIGH);
  digitalWrite(pinMotorDIR2, LOW);
}

void loop() {
  float value = get_readings();
  follow_line(value);
  delay(150);
}

int get_readings() {

  int IR1_readings[N_READINGS];
  int IR2_readings[N_READINGS];
  int IR3_readings[N_READINGS];
  int IR4_readings[N_READINGS];
  int IR5_readings[N_READINGS];

  float weighted_averages[N_READINGS];

  for(int i = 0; i < N_READINGS; i++)
  {
    IR1_readings[i] = analogRead(IR1);
    IR2_readings[i] = analogRead(IR2);
    IR3_readings[i] = analogRead(IR3);
    IR4_readings[i] = analogRead(IR4);
    IR5_readings[i] = analogRead(IR5);
    weighted_averages[i] = (IR1_readings[i]*WEIGHT_1)+(IR2_readings[i]*WEIGHT_2)+(IR3_readings[i]*WEIGHT_3)+(IR4_readings[i]*WEIGHT_4)+(IR5_readings[i]*WEIGHT_5);
  }

  float sum_of_averages = 0;

  for(int i = 0; i < N_READINGS; i++) 
  {
    sum_of_averages += weighted_averages[i];
  }

  return sum_of_averages/N_READINGS;
}

void follow_line(float value) {
  int base_speed = 80;  // Base speed for the motors
  int error = value-centro;  //  Error is the value from the sensor 
  P = error;      //   Proportional is the value of error (present)
  D = error - lasterror; // Derivative is error - lasterror prevent big modifications (future)
  lasterror = error;

  float Kp = 0.08;
  //float Kd = 0.2;
  int adjustment = 0;
  
  
  adjustment = P * Kp; //+ D * Kd ; // Adjust for speed

  if(adjustment + base_speed > 125){
    adjustment = 125;
  }
  
  int left_speed = base_speed - adjustment;
  int right_speed = base_speed + adjustment;

  // Clamp motor speeds to ensure they are within valid PWM range
  left_speed = constrain(left_speed, 0, 255);
  right_speed = constrain(right_speed, 0, 255);

  analogWrite(enableE, left_speed);
  analogWrite(enableD, right_speed);


  delay(2000);
  //Serial.print("Left Speed: ");
  //Serial.print(left_speed);
  //Serial.print(" | Right Speed: ");
  //Serial.print(right_speed);
  //Serial.print(" | Value: ");
  //Serial.println(value);

  Serial.println("error = " + String(error) );
  Serial.println("P = " + String(P) );
  Serial.println("D = " + String(D) );
  Serial.println(" PID adjustment: " + String(adjustment));
}


void calibrate(){
  float sum_of_averages = 0;
  int N = 200;
  for(int i = 0; i < N; i++) 
  {
    sum_of_averages += get_readings();
  }

  centro = sum_of_averages/N;
}

