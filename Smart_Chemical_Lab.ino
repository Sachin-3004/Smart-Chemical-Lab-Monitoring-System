#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ===== DHT22 Setup =====
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ===== Sensor Pins =====
#define GAS_SENSOR A0
#define LDR_SENSOR A1

// ===== Output Pins =====
#define GAS_LED 6
#define TEMP_LED 7
#define BUZZER 8

// ===== LCD Setup =====
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  Serial.begin(9600);

  dht.begin();

  pinMode(GAS_LED, OUTPUT);
  pinMode(TEMP_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("CHEM LAB");
  lcd.setCursor(0, 1);
  lcd.print("MONITORING...");
  delay(2000);

  lcd.clear();
}

void loop() {

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  int gasValue = analogRead(GAS_SENSOR);
  int lightValue = analogRead(LDR_SENSOR);

  // ===== Display Normal Data =====
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C ");

  lcd.print("H:");
  lcd.print(hum);

  lcd.setCursor(0, 1);
  lcd.print("Gas:");
  lcd.print(gasValue);

  delay(1500);

  // ===== GAS ALERT =====
  if (gasValue > 400) {

    digitalWrite(GAS_LED, HIGH);
    digitalWrite(BUZZER, HIGH);

    lcd.clear();
    lcd.print("GAS LEAK!");
    lcd.setCursor(0, 1);
    lcd.print("Evacuate Lab");

    delay(2000);
  }
  else {
    digitalWrite(GAS_LED, LOW);
    digitalWrite(BUZZER, LOW);
  }

  // ===== HIGH TEMPERATURE ALERT =====
  if (temp > 45) {

    digitalWrite(TEMP_LED, HIGH);
    digitalWrite(BUZZER, HIGH);

    lcd.clear();
    lcd.print("TEMP HIGH!");
    lcd.setCursor(0, 1);
    lcd.print("Cooling Req.");

    delay(2000);
  }
  else {
    digitalWrite(TEMP_LED, LOW);
  }

  // ===== LOW LIGHT ALERT =====
  if (lightValue > 800) {   // Dark condition

    digitalWrite(BUZZER, HIGH);

    lcd.clear();
    lcd.print("LOW LIGHT!");
    lcd.setCursor(0, 1);
    lcd.print("Check Lights");

    delay(2000);
  }

  digitalWrite(BUZZER, LOW);

  lcd.clear();
}
