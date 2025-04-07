#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

#define RFID_SS_PIN 10
#define RFID_RST_PIN 9
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);

#define LED_GREEN 8
#define LED_RED A5
#define BUZZER_PIN A4
#define FLAME_SENSOR A0
#define SOUND_SENSOR A3

const int flameThreshold = 150;
const int soundBaseValue = 617;
const int soundTolerance = 6;

bool isLocked = false;
bool alarmActive = false;
String activeAlarmType = "";

byte authorizedUID[] = {0x12, 0x5D, 0x98, 0xD4};
const byte authorizedUIDLength = 4;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("UNLOCKED");

  SPI.begin();
  rfid.PCD_Init();

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FLAME_SENSOR, INPUT);
  pinMode(SOUND_SENSOR, INPUT);

  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  checkFlameSensor();
  checkSoundSensor();
  checkRFID();
  updateAlarmState();
  delay(100);
}

void checkFlameSensor() {
  int flameValue = analogRead(FLAME_SENSOR);

  if (isLocked) {
    if (flameValue < flameThreshold) {
      triggerAlarm("FLAME ALARM!");
    }
  } else {
    if (flameValue < flameThreshold) {
      digitalWrite(BUZZER_PIN, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FLAME ALARM!");
    } else {
      digitalWrite(BUZZER_PIN, LOW);
      unlockSystem();
    }
  }
}

void checkSoundSensor() {
  if (!isLocked || alarmActive) return;

  int soundValue = analogRead(SOUND_SENSOR);
  Serial.print("Sound sensor analog: ");
  Serial.println(soundValue);

  if (soundValue > soundBaseValue + soundTolerance || soundValue < soundBaseValue - soundTolerance) {
    triggerAlarm("SOUND ALARM!");
  }
}

void checkRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  bool tagIsAuthorized = true;
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (i >= authorizedUIDLength || rfid.uid.uidByte[i] != authorizedUID[i]) {
      tagIsAuthorized = false;
      break;
    }
  }

  if (tagIsAuthorized && rfid.uid.size == authorizedUIDLength) {
    if (isLocked) unlockSystem();
    else lockSystem();
  } else {
    if (!isLocked) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("UNAUTHORIZED TAG");
      delay(1000);  // Show message briefly
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("UNLOCKED");
    }
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void lockSystem() {
  isLocked = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LOCKED");

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  alarmActive = false;
  activeAlarmType = "";
  digitalWrite(BUZZER_PIN, LOW);
}

void unlockSystem() {
  isLocked = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UNLOCKED");
  lcd.setCursor(0, 1);
  lcd.print("                ");

  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);
  alarmActive = false;
  activeAlarmType = "";
  digitalWrite(BUZZER_PIN, LOW);
}

void triggerAlarm(String alarmType) {
  if (!alarmActive) {
    Serial.println("ALARM TRIGGERED: " + alarmType);
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print(alarmType);
    alarmActive = true;
    activeAlarmType = alarmType;
  }
}

void updateAlarmState() {
  if (alarmActive) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_RED, LOW);
    delay(100);
    digitalWrite(LED_RED, HIGH);
    delay(100);
  }
}
