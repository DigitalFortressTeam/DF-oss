//pid speed
const float speed = 255;
float l_motor_speed, r_motor_speed;
float speed_differance;


//motors
const byte r_motor = 4;
const byte l_motor = 5;


//pid sensors
const byte r_sensor = A1;
const byte l_sensor = A2;


//full turn sensors
const byte wide_r_sensor = 6;
const byte wide_l_sensor = 7;


//pid constants
const float kp = 0.7;
const float ki = 0.001;
const float kd = 14;


//pid variables
float derivative, error, last_error, integral = 0;


//pid variables multiplied by pid constants
float final_derivative, final_integral, final_error;


//pid final calculations
float pid;

//close sensors readers
int r_sensor_reading;
int l_sensor_reading;


void Pidreading(int r_sensor_read, int l_sensor_read) {
  //pid variables calculation
  error = r_sensor_read - l_sensor_read;
  integral = error + integral;
  derivative = error - last_error;


  //multiplying pid variables with pid constants
  final_error = error * kp;
  final_integral = integral * ki;
  final_derivative = derivative * kd;


  //calculating pid
  pid = final_derivative + final_error + final_integral;


  //finding and using the speed of motors
  speed_differance = byte(map(pid, 0, 1023, 0, 255));
  r_motor_speed = speed + speed_differance;
  l_motor_speed = speed - speed_differance;
  analogWrite(r_motor, r_motor_speed);
  analogWrite(l_motor, l_motor_speed);


  //changing the error into last error
  last_error = error;


  //adding delay
  delay(5);
}

void Full_Right_Turn(int r_sensor_read, int l_sensor_read) {
  //reading the wide right sensor
  bool wide_r_sensor_reader = digitalRead(wide_r_sensor);


  //comparing if it catches light or not
  if (wide_r_sensor_reader = 0) {

    //launches l motor for a short period of time to make a ligh turn
    analogWrite(r_motor, 0);
    analogWrite(l_motor, 255);
    delay(50);
    while (r_sensor_read > 100 or l_sensor_read > 100) {

      //keeping the l motor on until the close sensors are on the track again
      analogWrite(r_motor, 0);
      analogWrite(l_motor, 255);
    }
  }
}



void Full_left_Turn(int r_sensor_read, int l_sensor_read) {
  //reading the wide left sensor
  bool wide_l_sensor_reader = digitalRead(wide_l_sensor);

  //checking if it catches light or not
  if (wide_l_sensor_reader = 0) {


    analogWrite(r_motor, 255);
    analogWrite(l_motor, 0);
    delay(50);
    while (r_sensor_read > 100 or l_sensor_read > 100) {
      analogWrite(r_motor, 255);
      analogWrite(l_motor, 0);
    }
  }
}
void setup() {
  // put your setup code here, to run once:
  pinMode(r_motor, OUTPUT);
  pinMode(l_motor, OUTPUT);
  pinMode(wide_l_sensor, INPUT);
  pinMode(wide_r_sensor, INPUT);
}

void loop() {
  //reading the close sensors
  r_sensor_reading = analogRead(r_sensor);
  l_sensor_reading = analogRead(l_sensor);

  //applying the funcions
  Full_left_Turn(r_sensor_reading, l_sensor_reading);
  Full_Right_Turn(l_sensor_reading, l_sensor_reading);
  Pidreading(r_sensor_reading, l_sensor_reading);
}