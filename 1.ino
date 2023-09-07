int hh = 0, mm = 0, ss = 0, ms = 0;
bool timerStart = false;
String totalTime;
String fastestLap;

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

#include <LiquidCrystal.h>
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Welcome  To  ");
  lcd.setCursor(0, 1);
  lcd.print("   Stopwatch   ");
  delay(2000);
  lcd.clear();


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


  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  delay(1000);
}

int text = 0;



void loop() {
  timerStart = true;  // Start stopwatch
  lcd.setCursor(0, 0);
  lcd.print("   Laptime:   ");
  lcd.setCursor(2, 1);
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
  Serial.print("Distance: ");
  Serial.println(distance);


  if (distance <= 12) {
    totalTime = String((hh / 10) % 10) + String(hh % 10) + String(":") + String((mm / 10) % 10) + String(mm % 10) + String(":") + String((ss / 10) % 10) + String(ss % 10) + String(":") + String((ms / 100) % 10) + String((ms / 10) % 10) + String(ms % 10);
    Serial.print(totalTime);

    // char *token;
    // char *totalTime;
    // const char *delimiter =",";
    // token = strtok(totalTime, delimiter);
    // while (token != NULL) {
    //   Serial.println(token);
    //   token=strtok(NULL, delimiter);
  


          
  }
  
  delay(10000);
  ms = 0, ss = 0, mm = 0, hh = 0;  // Reset stopwatch
}
}

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