// ===========================================================
// 🔐 SMART DOOR LOCK SYSTEM - RFID + KEYPAD + LCD + BUZZER + LEDs
// Compatible Hardware: Arduino UNO, MFRC522, 4x4 Keypad, 16x2 I2C LCD, Active Buzzer
// ===========================================================

// ====== Include Libraries ======
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>

// ====== Pin Definitions ======
#define SS_PIN 10     // RFID SDA
#define RST_PIN 9     // RFID RST
#define GREEN_LED 6
#define RED_LED 5
#define BUZZER 8

// ====== Initialize Components ======
LiquidCrystal_I2C lcd(0x27, 16, 2); // Common I2C address for LCD (0x27 or 0x3F)
MFRC522 rfid(SS_PIN, RST_PIN);

// ====== Keypad Setup ======
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {0, 1, 2, 4}; // D0, D1, D2, D4

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ====== Stored RFID UID and Password ======
String validUID = "C3 4A 9D 27";     // Replace with your card/tag UID
String setPassword = "1234";         // Default password

// ====== Function Declarations ======
void accessGranted();
void accessDenied();
String readRFID();
bool checkPassword();

void setup() {
  // Initialize serial monitor
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" SMART DOOR LOCK ");
  lcd.setCursor(0, 1);
  lcd.print("   Initializing  ");
  delay(2000);
  lcd.clear();

  // Initialize LEDs & Buzzer
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Initialize RFID
  SPI.begin();
  rfid.PCD_Init();
  lcd.setCursor(0, 0);
  lcd.print("Scan your card");
}

void loop() {
  // Wait for RFID card
  String uid = readRFID();

  if (uid.length() > 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card UID:");
    lcd.setCursor(0, 1);
    lcd.print(uid);
    delay(1000);

    // Check if RFID is valid
    if (uid == validUID) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RFID OK");
      lcd.setCursor(0, 1);
      lcd.print("Enter Password:");
      delay(1000);

      // Prompt for password
      if (checkPassword()) {
        accessGranted();
      } else {
        accessDenied();
      }
    } else {
      accessDenied();
    }

    lcd.clear();
    lcd.print("Scan your card");
  }
}

// ====== Function Definitions ======

String readRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return "";
  }

  String content = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(rfid.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  rfid.PICC_HaltA();
  return content.substring(1); // Remove leading space
}

bool checkPassword() {
  String inputPassword = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");

  while (true) {
    char key = keypad.getKey();
    if (key) {
      if (key == '#') {  // '#' to confirm
        break;
      } else if (key == '') {  // '' to clear input
        inputPassword = "";
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
      } else {
        inputPassword += key;
        lcd.print('*'); // Show asterisk for each keypress
      }
    }
  }

  if (inputPassword == setPassword) {
    return true;
  } else {
    return false;
  }
}

void accessGranted() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Access Granted ");
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(300);
  digitalWrite(BUZZER, LOW);
  delay(2000);
  digitalWrite(GREEN_LED, LOW);
  lcd.clear();
}

void accessDenied() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Access Denied ");
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(1000);
  digitalWrite(BUZZER, LOW);
  digitalWrite(RED_LED, LOW);
  lcd.clear();
}
