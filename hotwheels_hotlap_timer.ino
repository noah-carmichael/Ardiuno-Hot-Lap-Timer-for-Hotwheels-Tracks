// timer code sourced from https://www.youtube.com/watch?v=jacncr5Q0Pc

int hh = 0, mm = 0, ss = 0, ms = 0;
bool timerStart = false;
String totalTime;
String fastestLap = "99:99:99:999";
char* token;
const char* delimiter = ":";

// button
const int buttonPin = 2;
int buttonState = 0;

bool resetFastest = true;
bool firstRun = true;

char* tokens[4];  // Array to store the tokens
char* tokensHigh[4];
char* tokenHigh;  // Array to store the tokens
int index = 0;
int index2 = 0;


// millis
int period1 = 0.2;
int period2 = 0.1;
unsigned long time_now = 0;

// ultrasonic sensor
const int trigPin = 12;
const int echoPin = 13;
long duration;
int distance;
int num;

// lcd
#include <LiquidCrystal.h>
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting Hotlap ");
  lcd.setCursor(0, 1);
  lcd.print("    Timer...  ");
  delay(1000);
  lcd.clear();

  // entirely copied from the video, not sure what this section does
  noInterrupts();  // disable all interrupts
  TCCR1A = 0;      // set entire TCCR1A register to 0  //set timer1 interrupt at 1kHz  // 1 ms
  TCCR1B = 0;      // same for TCCR1B
  TCNT1 = 0;       // set timer count for 1khz increments
  OCR1A = 1999;    // = (16*10^6) / (1000*8) - 1
  //had to use 16 bit timer1 for this bc 1999>255, but could switch to timers 0 or 2 with larger prescaler
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);   // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  interrupts();  // enable

  // declare ultrasonic bits
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  delay(1000);
}

int text = 0;



void loop() {
  // lcd display stuff
  lcd.setCursor(0, 0);
  lcd.print("CUR   ");
  lcd.setCursor(4, 0);
  lcd.print((hh / 10) % 10);
  lcd.print(hh % 10);
  lcd.print(":");
  lcd.print((mm / 10) % 10);
  lcd.print(mm % 10);
  lcd.print(":");
  lcd.print((ss / 10) % 10);
  lcd.print(ss % 10);
  lcd.print(":");
  lcd.print((ms / 100) % 10);
  lcd.print((ms / 10) % 10);
  lcd.print(ms % 10);

  // fastest lap
  lcd.setCursor(0, 1);
  lcd.print("BST   ");
  lcd.setCursor(4, 1);
  lcd.print(fastestLap);


  // use ultrasonic
  digitalWrite(trigPin, LOW);
  if (millis() >= time_now + period1) {
    time_now += period1;
    digitalWrite(trigPin, HIGH);
  }
  if (millis() >= time_now + period2) {
    time_now += period2;
    digitalWrite(trigPin, LOW);
  }
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.println(distance);


  // reset fastest lap
  buttonState = digitalRead(buttonPin);
  // Serial.println(buttonState);
  if (buttonState == LOW) {
    fastestLap = "99:99:99:999";
    resetFastest = true;
    Serial.println("RESET");
  }

  // check for if a car comes by
  if (distance <= 3) {
    if (firstRun == true) {
      timerStart = true;
      firstRun = false;
    } else {  // Start stopwatch
      totalTime = String((hh / 10) % 10) + String(hh % 10) + String(":") + String((mm / 10) % 10) + String(mm % 10) + String(":") + String((ss / 10) % 10) + String(ss % 10) + String(":") + String((ms / 100) % 10) + String((ms / 10) % 10) + String(ms % 10);
      // Serial.println(totalTime);
      Serial.println(totalTime);

      char charBuf[13];
      totalTime.toCharArray(charBuf, 13);

      token = strtok(charBuf, delimiter);

      // Store tokens in the array
      while (token != NULL && index < 4) {
        tokens[index] = token;
        index++;
        token = strtok(NULL, ":");
      }

      // for the fastest lap
      char charCringe[13];
      fastestLap.toCharArray(charCringe, 13);
      tokenHigh = strtok(charCringe, delimiter);
      while (tokenHigh != NULL && index2 < 4) {
        tokensHigh[index2] = tokenHigh;
        index2++;
        tokenHigh = strtok(NULL, ":");
      }

      for (int i = 0; i < index; i++) {

        String toke1 = tokens[i];
        String toke2 = tokensHigh[i];
        if (index < 3) {
          if (toke1.substring(0, 2).toInt() > toke2.substring(0, 2).toInt()) {
            break;
          }
          if (toke1.substring(0, 2).toInt() < toke2.substring(0, 2).toInt()) {
            fastestLap = totalTime;
            // Serial.println(toke1.substring(0, 2));
            // Serial.println(toke2.substring(0, 2));
          }

        } else {
          if (toke1.substring(0, 2).toInt() > toke2.substring(0, 2).toInt()) {
            break;
          }
          if (toke1.substring(0, 3).toInt() < toke2.substring(0, 3).toInt()) {
            fastestLap = totalTime;
            // Serial.println(toke1.substring(0, 3));
            // Serial.println(toke2.substring(0, 3));
          }
        }
      }
    }


    // delay to not detect the car twice
    delay(700);
    ms = 700, ss = 0, mm = 0, hh = 0;  // Reset stopwatch
  }
}

// code from video
ISR(TIMER1_COMPA_vect) {
  if (timerStart == true) {
    ms = ms + 1;
    if (ms > 999) {
      ms = 0;
      ss = ss + 1;
      if (ss > 59) {
        ss = 0;
        mm = mm + 1;
      }
      if (mm > 59) {
        mm = 0;
        hh = hh + 1;
      }
    }
  }
}
