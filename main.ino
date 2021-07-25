#include <Servo.h>

Servo fingers[5];
int positions[5];
int start_position = 0;

int max_analog_dta      = 300;              // max analog data
int min_analog_dta      = 100;              // min analog data
int static_analog_dta   = 0;                // static analog data

void servo_attach() {
  for (int i = 0; i < 5; i++) {
    fingers[i].attach(i + 2);
  }
}

// get analog value
int getAnalog(int pin)
{
  long sum = 0;



  for (int i = 0; i < 32; i++)
  {
    sum += analogRead(pin);

  }

  int dta = sum >> 5;

  max_analog_dta = dta > max_analog_dta ? dta : max_analog_dta;       // if max data
  min_analog_dta = min_analog_dta > dta ? dta : min_analog_dta;       // if min data

  return sum >> 5;


}

void write_servo(int finger, int pos) {
  fingers[finger].write(pos);
  positions[finger] = pos;
}

void write_to_all(int pos) {
  for (int i = 0; i < 5; i++) {
    write_servo(i, pos);
  }
}

void initialize() {
  Serial.print("Initialize");
  delay(200);
  servo_attach();
  write_to_all(start_position);
  delay(200);
  Serial.print("Done");
}

void setup() {
  Serial.begin(115200);

  long sum = 0;

  for (int i = 0; i <= 10; i++)
  {
    for (int j = 0; j < 100; j++)
    {
      sum += getAnalog(A0);
      delay(1);
    }

  }

  sum /= 1100;

  static_analog_dta = sum;

  initialize();

  Serial.print("static_analog_dta = ");
  Serial.println(static_analog_dta);
}

void move_finger(int finger, int pos, int speed) {
  int i = 0;

  if (pos > 180 || pos < 0)
    return;

  if (positions[finger] < pos) {
    for (i = positions[finger]; i < pos; i++) {
      write_servo(finger, i);
      delay(speed);
    }
  }
  if (positions[finger] > pos) {
    for (i = positions[finger]; i > pos; i--) {
      write_servo(finger, i);
      delay(speed);
    }
  }
}

int calculate_position(int s) {
  switch ( s ) {
    case 7:
      return 90;
    case 8:
      return 120;
    case 9:
      return 150;
    case 10:
      return 180;
  }
}

int calculate_speed(int s) {
  switch ( s ) {
    case 7:
      return 5;
    case 8:
      return 4;
    case 9:
      return 3;
    case 10:
      return 2;
  }
}

void grep(int strength) {
  int pos = calculate_position(strength);
  int speed = calculate_speed(strength);
  for (int i = 0; i < 5; i++) {
    move_finger(i, pos, speed);
  }
}

void relax() {
  for (int i = 0; i < 5; i++) {
    move_finger(i, 0, 5);
  }
}

int level       = 5;
int level_buf   = 5;
bool greped = false;

void loop() {

  int val = getAnalog(A0);                    // get Analog value

  int level2;

  if (val > static_analog_dta)                // larger than static_analog_dta
  {
    level2 = 5 + map(val, static_analog_dta, max_analog_dta, 0, 5);
  }
  else
  {
    level2 = 5 - map(val, min_analog_dta, static_analog_dta, 0, 5);
  }

  // to smooth the change of led bar
  if (level2 > level)
  {
    level++;
    Serial.println(level);
    if (level > 6 && !greped) {
      grep(level);
      greped = true;
      delay(2000);
    }
  }
  else if (level2 < level)
  {
    level--;
    Serial.println(level);
    if (level < 5 && greped) {
      relax();
      greped = false;
      delay(2000);
    }
  }

  delay(10);

}
