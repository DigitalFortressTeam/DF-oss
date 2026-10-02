// Line tracker: follows the line with a PID controller on the two close sensors,
// and makes sharp turns when one of the wide sensors sees the line.

// ---------------------------------------------------------------------------
// Pins
// ---------------------------------------------------------------------------
// Motors
constexpr byte RIGHT_MOTOR_PIN = 4;
constexpr byte LEFT_MOTOR_PIN = 5;

// Close sensors (used by the PID)
constexpr byte RIGHT_SENSOR_PIN = A1;
constexpr byte LEFT_SENSOR_PIN = A2;

// Wide sensors (used for full turns)
constexpr byte WIDE_RIGHT_SENSOR_PIN = 6;
constexpr byte WIDE_LEFT_SENSOR_PIN = 7;

// ---------------------------------------------------------------------------
// Speed
// ---------------------------------------------------------------------------
constexpr float BASE_SPEED = 255;
float leftMotorSpeed, rightMotorSpeed;
float speedDifference;

// ---------------------------------------------------------------------------
// PID
// ---------------------------------------------------------------------------
// Constants
constexpr float KP = 0.7;
constexpr float KI = 0.001;
constexpr float KD = 14;

// Variables
float derivative, error, lastError, integral = 0;

// Variables multiplied by their constants
float proportionalTerm, integralTerm, derivativeTerm;

// Final PID output
float pidOutput;

// Latest close sensor readings
int rightSensorReading;
int leftSensorReading;

// ---------------------------------------------------------------------------
// Full turns
// ---------------------------------------------------------------------------
constexpr int FULL_TURN_ON_TRACK_THRESHOLD = 100;
constexpr int FULL_TURN_KICK_MOTOR_SPEED = 255;
constexpr int FULL_TURN_KICK_DURATION_MS = 50;

constexpr int PID_LOOP_DELAY_MS = 5;

// Steers the robot by comparing both close sensors.
void applyPidSteering(int rightSensorValue, int leftSensorValue) {
  // PID variables calculation
  error = rightSensorValue - leftSensorValue;
  integral = error + integral;
  derivative = error - lastError;


  // Multiplying PID variables with PID constants
  proportionalTerm = error * KP;
  integralTerm = integral * KI;
  derivativeTerm = derivative * KD;


  // Calculating PID
  pidOutput = derivativeTerm + proportionalTerm + integralTerm;


  // Finding and using the speed of motors
  speedDifference = byte(map(pidOutput, 0, 1023, 0, 255));
  rightMotorSpeed = BASE_SPEED + speedDifference;
  leftMotorSpeed = BASE_SPEED - speedDifference;
  analogWrite(RIGHT_MOTOR_PIN, rightMotorSpeed);
  analogWrite(LEFT_MOTOR_PIN, leftMotorSpeed);


  // Remembering the error for the next derivative
  lastError = error;


  // Adding delay
  delay(PID_LOOP_DELAY_MS);
}

// Sharp turn to the right, meant to run when the wide right sensor catches the line.
void performFullRightTurn(int rightSensorValue, int leftSensorValue) {
  // Reading the wide right sensor
  bool wideRightSensorReading = digitalRead(WIDE_RIGHT_SENSOR_PIN);


  // Comparing if it catches light or not
  // NOTE: this is an assignment ("=") and not a comparison ("=="), kept as in the original code.
  // It always evaluates to false, so this turn never happens.
  if (wideRightSensorReading = 0) {

    // Runs the left motor alone for a short period of time to make a light turn
    analogWrite(RIGHT_MOTOR_PIN, 0);
    analogWrite(LEFT_MOTOR_PIN, FULL_TURN_KICK_MOTOR_SPEED);
    delay(FULL_TURN_KICK_DURATION_MS);
    while (rightSensorValue > FULL_TURN_ON_TRACK_THRESHOLD or leftSensorValue > FULL_TURN_ON_TRACK_THRESHOLD) {

      // Keeping the left motor on until the close sensors are on the track again
      analogWrite(RIGHT_MOTOR_PIN, 0);
      analogWrite(LEFT_MOTOR_PIN, FULL_TURN_KICK_MOTOR_SPEED);
    }
  }
}


// Sharp turn to the left, meant to run when the wide left sensor catches the line.
void performFullLeftTurn(int rightSensorValue, int leftSensorValue) {
  // Reading the wide left sensor
  bool wideLeftSensorReading = digitalRead(WIDE_LEFT_SENSOR_PIN);

  // Checking if it catches light or not
  // NOTE: assignment ("=") instead of comparison ("=="), kept as in the original code,
  // so this turn never happens either.
  if (wideLeftSensorReading = 0) {


    analogWrite(RIGHT_MOTOR_PIN, FULL_TURN_KICK_MOTOR_SPEED);
    analogWrite(LEFT_MOTOR_PIN, 0);
    delay(FULL_TURN_KICK_DURATION_MS);
    while (rightSensorValue > FULL_TURN_ON_TRACK_THRESHOLD or leftSensorValue > FULL_TURN_ON_TRACK_THRESHOLD) {
      analogWrite(RIGHT_MOTOR_PIN, FULL_TURN_KICK_MOTOR_SPEED);
      analogWrite(LEFT_MOTOR_PIN, 0);
    }
  }
}

void setup() {
  pinMode(RIGHT_MOTOR_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_PIN, OUTPUT);
  pinMode(WIDE_LEFT_SENSOR_PIN, INPUT);
  pinMode(WIDE_RIGHT_SENSOR_PIN, INPUT);
}

void loop() {
  // Reading the close sensors
  rightSensorReading = analogRead(RIGHT_SENSOR_PIN);
  leftSensorReading = analogRead(LEFT_SENSOR_PIN);

  // Applying the functions
  performFullLeftTurn(rightSensorReading, leftSensorReading);
  // NOTE: the left reading is passed for both arguments, kept as in the original code.
  performFullRightTurn(leftSensorReading, leftSensorReading);
  applyPidSteering(rightSensorReading, leftSensorReading);
}
