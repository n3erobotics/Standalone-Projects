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
int pinMotorESQ = 6;
int pinMotorDIR = 5;

void setup() {
  Serial.begin(9600);
  pinMode(pinMotorDIR, OUTPUT);
  pinMode(pinMotorESQ, OUTPUT);
}

void loop() {
  float value = get_readings();
  follow_line(value);
  delay(150);
}

float get_readings() {
  int IR1_readings[N_READINGS];
  int IR2_readings[N_READINGS];
  int IR3_readings[N_READINGS];
  int IR4_readings[N_READINGS];
  int IR5_readings[N_READINGS];

  float weighted_averages[N_READINGS];

  for(int i = 0; i < N_READINGS; i++) {
    IR1_readings[i] = analogRead(IR1);
    IR2_readings[i] = analogRead(IR2);
    IR3_readings[i] = analogRead(IR3);
    IR4_readings[i] = analogRead(IR4);
    IR5_readings[i] = analogRead(IR5);
    weighted_averages[i] = (IR1_readings[i]*WEIGHT_1) + 
                           (IR2_readings[i]*WEIGHT_2) + 
                           (IR3_readings[i]*WEIGHT_3) + 
                           (IR4_readings[i]*WEIGHT_4) + 
                           (IR5_readings[i]*WEIGHT_5);
  }

  float sum_of_averages = 0;

  for(int i = 0; i < N_READINGS; i++) {
    sum_of_averages += weighted_averages[i];
  }

  return sum_of_averages / N_READINGS;
}

void follow_line(float value) {
  int base_speed = 150;  // Base speed for the motors
  int error = value;  //  Error is the value from the sensor 
  int P = error;      //   Proportional is the value of error (present)
  int I = P + error;  //   Integral is the sum of the errors (pass)
  int lasterror = error; 
  int D = erro - lasterror; // Derivative is error - lasterror prevent big modifications (future)
  
  int Kp = 1;
  int Ki = 0.5;
  int Kd = 0.5;

  int adjustment = P * Kp + I * Ki + D * Kd ; // Adjust for speed
  
  int left_speed = base_speed - adjustment;
  int right_speed = base_speed + adjustment;

  // Clamp motor speeds to ensure they are within valid PWM range
  left_speed = constrain(left_speed, 0, 255);
  right_speed = constrain(right_speed, 0, 255);

  analogWrite(pinMotorESQ, left_speed);
  analogWrite(pinMotorDIR, right_speed);

  Serial.print("Left Speed: ");
  Serial.print(left_speed);
  Serial.print(" | Right Speed: ");
  Serial.print(right_speed);
  Serial.print(" | Value: ");
  Serial.println(value);
}

