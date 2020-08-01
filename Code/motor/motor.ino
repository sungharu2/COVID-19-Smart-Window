void setup() {
  Serial.begin(9600);

  // magnetic door sensor
  pinMode(4, INPUT);  // CLOSED
  pinMode(5, INPUT);  // OPENED
  // DC motor 
  // 8:GND(IN1) 9:VCC(IN2)
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  // DC motor init
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}

void motor_on_left() {
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
}
void motor_on_right() {
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
}
void motor_off() {
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}

String command = "";

void init_command_buffer() {
  command = "";
  while(Serial.available() > 0) {
    Serial.read();
    Serial.print(".");
  }
  Serial.println("buffer init done");
}

bool isOpened = false;
bool magnet_ignore_state = true;

void testFunc();

void loop() {
  //testFunc();
  
  if (Serial.available() > 0) {
    char cTemp = Serial.read();
    
    // save character
    command.concat(cTemp);

    // get command at new line character
    if (cTemp == '\n') {
      command.trim();
      Serial.println("command input: " + command);

      // match command
      if (command == "open")  {
        if (isOpened) {
            Serial.println("already opened");
            init_command_buffer();
        }
        else {
          Serial.println("opening");
        }
        
        while (!isOpened) {
          motor_on_left();

          // Check magnet sensor
          if (digitalRead(5) == HIGH) {
            motor_off();
            Serial.println("done!");
            isOpened = true;
            // command and buffer 초기화
            init_command_buffer();
            break;
          }
        } 
      }
      else if (command == "close") {
        if (!isOpened) {
            Serial.println("already closed");
            init_command_buffer();
        } else {
          Serial.println("closing");
        }
        
        while (isOpened) {
          motor_on_right();

          // Check magnet sensor
          if (digitalRead(4) == HIGH) {
            motor_off();
            Serial.println("done!");
            isOpened = false;
            // command and buffer 초기화
            init_command_buffer();
            break;
          }
        }
      }
      else {
        Serial.println("Wrong command: " + command);
        // ignore wrong command
        init_command_buffer();
      }
    }
  }
  
}


void testFunc() {
  int count = 0;
  while(1) {
    while(1) {
      motor_on_left();
      if (digitalRead(5) == HIGH) break;
    }
    
    while(1) {
      motor_on_right();
      if (digitalRead(4) == HIGH) break;
    }
    count++;
    Serial.println(count);
  }
}
