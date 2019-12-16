/*
   Project Name: Cockroach
   Purpose: Form the robot as a vivid, lively organism by using the encoder and corresponding components.
   And try as much as possible to make it feels like a really alive cockroach.
   Significance: It demonstrates that it is possible of giving the power of living organisms to the combination of electronic components(Robots)
   It is just the start of training insects to bring us information from the spaces humans cannot go, and it will carry us on during the jouney of exploring the world of technology.
   Author Name: Geoffrey, Qin & Yvan, Xie
   Instruction: Find the robot that labelled "Geoffrey, Qin"
   Upload the software
   Place the robot in the middle of the room
   Plug robot's power in
   Leave it alone...
*/

volatile int blocked = LOW;
long hit_time;
int normal_round = 0;
int decision_round = 0;
int val_left = analogRead(A2);   // store the variable "val_left" as the data that the analog Input pin 2 received and translated by 10 A/D converter.
int val_right = analogRead(A4);  // store the variable "val_right" as the data that the analog Input pin 4 received and translated by 10 A/D converter.
int difference = val_left - val_right; // between -20 and 20 meansno significant difference
// greater than 20 means left side darker
int dark = 0;
void setup() {
  pinMode(3, OUTPUT); //IN1
  pinMode(5, OUTPUT); //IN2
  pinMode(6, OUTPUT); //IN1
  pinMode(11, OUTPUT); //IN2
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);
  attachInterrupt(0, jammed, CHANGE);
}

void forward() {
  digitalWrite(3, LOW); // move forward
  analogWrite(5, 127);;
  analogWrite(6, 127);
  digitalWrite(11, LOW);
}
void backward() {
  analogWrite(3, 127); // move backward
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  analogWrite(11, 127);
}
void turn_left() {
  analogWrite(3, 127); // turn left
  digitalWrite(5, LOW);
  analogWrite(6, 127);
  digitalWrite(11, LOW);
}
void turn_right() {
  digitalWrite(3, LOW); //turn right
  analogWrite(5, 127);
  digitalWrite(6, LOW);
  analogWrite(11, 127);
}
void brake() {
  digitalWrite(3, LOW);  //stop
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(11, LOW);

}
void jammed() {
  blocked = ! blocked;
  hit_time = millis() + 2000;  //record the time when wheel are moving

}
void evasion() {
  if (millis () > hit_time) {
    if (val_right >= 120 && val_left >= 120) {
      digitalWrite(3, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(11, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(8, HIGH);
      delay(100);
      digitalWrite(4, LOW);
      digitalWrite(8, LOW);
      dark = 1;
    }
    else {
      if (dark == 0) {
        digitalWrite(10, HIGH);  // back up and avoid the obstruction
        brake();
        delay(200);
        backward();
        delay(1000);
        turn_right();
        delay(500);
        forward();
        delay(2000);
        digitalWrite(10, LOW);
      }
    }
  }
  else {
    if (dark == 0) {      // move forward because the way is clear
      digitalWrite(12, HIGH);
      forward();
      delay(2000);
      digitalWrite(12, LOW);
    }
  }
}

void loop() {
  int val_left = analogRead(A2);   // store the variable "val_left" as the data that the analog Input pin 2 received and translated by 10 A/D converter.
  int val_right = analogRead(A4);  // store the variable "val_right" as the data that the analog Input pin 4 received and translated by 10 A/D converter.
  int difference = val_left - val_right; // between -60 and 40 meansno significant difference
  // greater than 40 means left side darker
  // less than -60 means right side darker
  int target_time = millis() - hit_time;  //track the wheel by tracking the time
  Serial.println(difference);        //print the conditions to the computer
  Serial.print("Left:");
  Serial.print(val_left);
  Serial.print("      ");
  Serial.print("Right:");
  Serial.print(val_right);
  Serial.println(" ");
  Serial.print("hit:");
  Serial.println(hit);
  Serial.print("target time:");
  Serial.println(target_time);
  delay(300);


  if (val_right >= 120 && val_left >= 120) {   //check if it is dark enough
    if (millis() > hit_time) {        //check if it hited something
      digitalWrite(3, LOW);              //turn off all motor and stand by
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(11, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(8, HIGH);
      delay(100);
      digitalWrite(4, LOW);
      digitalWrite(8, LOW);
      dark = 1;
    }
  }
  else {
    dark = 0;
    digitalWrite(4, LOW);
    digitalWrite(8, LOW);
    if (difference > 40) {         // Check is the left side significantly darker or not, if it is run the following commands
      digitalWrite(4, HIGH);
      digitalWrite(8, LOW);
      turn_left();
      delay(1000);
      brake();
      delay(200);
      forward();
      delay(1000);
      decision_round = 1;          // Store variable "decision_round" as 1
      digitalWrite(4, LOW);
      digitalWrite(8, LOW);
    }
    else if (difference < -60) {       // Check is the right side significantly darker or not, if it is run the following commands
      digitalWrite(4, LOW);
      digitalWrite(8, HIGH);
      turn_right();
      delay(1000);
      brake();
      delay(200);
      forward();
      delay(1000);
      decision_round = 1;         // Store variable "decision_round" as 1
      digitalWrite(4, LOW);
      digitalWrite(8, LOW);
    }

    else if (decision_round == 1) {
      evasion();
      decision_round = 0;       // Store variable "decision_round" as 0
      digitalWrite(4, LOW);
      digitalWrite(8, LOW);
    }
  }
  evasion();
}
