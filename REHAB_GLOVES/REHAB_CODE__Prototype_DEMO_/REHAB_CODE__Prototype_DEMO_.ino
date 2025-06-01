#include <HX710B.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ——— Pin Definitions ———
const int PIN_MOTOR_RELAY = 2;    // Relay IN1
const int PIN_VALVE_RELAY = 3;    // Relay IN2
const int PIN_PRESSURE_OUT = A0;  // HX710B DOUT
const int PIN_PRESSURE_SCK = 4;   // HX710B SCK
const int PIN_BUTTON      = 5;    // Start/Stop (pulled up)

// ——— Constants ———
const long MAX_PRESSURE    = 100000L;  // Pressure threshold to cut off inflation
const unsigned long T_INF  = 2000;     // Inflate (bend) duration in ms
const unsigned long T_REST = 3000;     // Relax duration in ms

// ——— Globals ———
HX710B pressureSensor(PIN_PRESSURE_OUT, PIN_PRESSURE_SCK);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust I²C address if needed
bool therapyActive = false;
int  cycleCount    = 0;

void setup() {
  pinMode(PIN_MOTOR_RELAY, OUTPUT);
  pinMode(PIN_VALVE_RELAY, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // Relays are active-LOW: drive HIGH to keep off
  digitalWrite(PIN_MOTOR_RELAY, HIGH);
  digitalWrite(PIN_VALVE_RELAY, HIGH);

  lcd.begin();
  lcd.backlight();
  lcd.print("Rehab Glove Ready");

  pressureSensor.begin();
  delay(1000);
}

void loop() {
  // — Toggle therapyActive when button is pressed (simple debounce)
  if (digitalRead(PIN_BUTTON) == LOW) {
    therapyActive = !therapyActive;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print( therapyActive ? "Therapy: ON" : "Therapy: OFF" );
    delay(200);  // debounce
  }

  if (therapyActive) {
    runOneCycle();
  }
}

// ——— Performs one inflate→rest cycle, updates LCD & cycleCount ———
void runOneCycle() {
  // Inflate phase (bend glove)
  lcd.setCursor(0,1);
  lcd.print("Bending...       ");
  digitalWrite(PIN_MOTOR_RELAY, LOW);
  digitalWrite(PIN_VALVE_RELAY, LOW);

  unsigned long start = millis();
  while (millis() - start < T_INF) {
    long p = pressureSensor.read();
    if (p > MAX_PRESSURE) {
      lcd.setCursor(0,1);
      lcd.print("Pressure HIGH!   ");
      break;
    }
  }

  // Deflate phase (relax glove)
  digitalWrite(PIN_MOTOR_RELAY, HIGH);
  digitalWrite(PIN_VALVE_RELAY, HIGH);
  lcd.setCursor(0,1);
  lcd.print("Relaxing...      ");
  delay(T_REST);

  // Update cycle count on top line
  cycleCount++;
  lcd.setCursor(0,0);
  lcd.print("Cycle: ");
  lcd.print(cycleCount);
}
