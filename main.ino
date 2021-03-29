#include <Servo.h>

Servo fingers[5];
int positions[5];
int start_position = 0;

void servo_attach() {
    for (int i = 0; i < 8; i++) {
        fingers[i].attach(i+2);
    }
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
    Serial.begin(9600);
    initialize();
}

void move_finger(int finger, int pos, int speed) {
    int i = 0;
    
    if (pos > 180 || pos < 0)
        return
 
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

void loop() {
    
}

