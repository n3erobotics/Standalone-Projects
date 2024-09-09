//  TOP SPEED MÁXIMA = 125 MOTORES DE 6V


#define N_READINGS 5

#define WEIGHT_1 0
#define WEIGHT_2 1
#define WEIGHT_3 2
#define WEIGHT_4 3
#define WEIGHT_5 4

int IR1 = A1;
int IR2 = A2;
int IR3 = A3;
int IR4 = A4;
int IR5 = A5;

int led_L = 13;

int P = 0;
int D = 0;
int lasterror = 0;

long int min_value = 2000;
long int max_value = 0;
long int zero = 0;
long int max = 1000;

int max_speed = 110;
int min_speed = 40;

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
  pinMode(led_L, OUTPUT);

  calibrate();
  delay(2000);
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

void follow_line(long int value) {
  int base_speed = 80;  // Base speed for the motors
  int error = value;  //  Error is the value from the sensor 
  P = error;      //   Proportional is the value of error (present)
  D = error - lasterror; // Derivative is error - lasterror prevent big modifications (future)
  lasterror = error;

  value = map(value, min_value, max_value, zero , max);

  float Kp = 0.65;
  float Kd = 0.5;
  int adjustment = 0;
  
  
  adjustment = P * Kp + D * Kd ; // Adjust for speed
  
  int left_speed = base_speed - adjustment;
  int right_speed = base_speed + adjustment;

  // Clamp motor speeds to ensure they are within valid PWM range
  left_speed = constrain(left_speed, min_speed, max_speed);
  right_speed = constrain(right_speed, min_speed, max_speed);

  analogWrite(enableE, left_speed);
  analogWrite(enableD, right_speed);

}


void calibrate(){
  float value = 0;
  int N = 3000;

  digitalWrite(led_L, HIGH);

  for(int i = 0; i < N; i++) 
  {
    value = get_readings();
    if(max_value < value ){
      max_value = value;
    }

    if(min_value > value){
      min_value = value;
    }
    delay(2);
  }

  digitalWrite(led_L, LOW);

}

