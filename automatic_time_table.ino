
#include <RTClib.h>
RTC_DS1307 rtc; //i decleared a rtc object.
//#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>

//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
DateTime Time;
byte buzzer = 8;
const int leftBtn = 9;
const int configBtn = 10;
const int rightBtn = 11;
int Hour = 0, Min = 0, Sec = 0, initialSecond = 0;
int alarmHours = 0, alarmMinites = 0, alarmSeconds = 0, setAlarmState = 0;
bool isSetAlarm = false, isSoundAlarm = false, isAlarm = false;
uint32_t alarmWaitTime = 0, lcdUpdateDelay = 0;

void setup() {
  Serial.begin(9600);
  rtc.begin();
  lcd.begin(16, 2);

  if (!rtc.begin()) {
    lcd.clear();
    lcd.print("rtc not found!");
  }
  else {
    lcd.clear();
    lcd.print("  initializing");
    lcd.setCursor(0, 1);
    lcd.print("     system");
  }

  // uncomment to set rtc time with the current time of your computer...
  //  if (Serial) {
  //    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //  }

  delay(2000);
  pinMode(buzzer, OUTPUT);
  pinMode(configBtn, INPUT_PULLUP);
  pinMode(leftBtn, INPUT_PULLUP);
  pinMode(rightBtn, INPUT_PULLUP);

  Time = rtc.now();
  int Hour = (Time.hour(), DEC);
  int Min = (Time.minute(), DEC);
  int Sec = (Time.second(), DEC);
}

void loop() {
  updateTimeVaribles();
  LcdUpdate();
  setMode();
  setAlarmTime();
  setIsAlarm();
  soundAlarm();
}

void soundAlarm() {
  if (millis() - alarmWaitTime > 200 && isAlarm) {
    alarmWaitTime = millis();
    isSoundAlarm = !isSoundAlarm;
    digitalWrite(buzzer, isSoundAlarm);
  }
}

void LcdUpdate() {
  if (isSetAlarm && millis() - lcdUpdateDelay > 100) {
    lcdUpdateDelay = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("set alarm ");
    lcd.print(setAlarmState == 1 ? "hours" : "");
    lcd.print(setAlarmState == 2 ? "minites" : "");
    lcd.print(setAlarmState == 3 ? "seconds" : "");
    lcd.setCursor(0, 1);
    lcd.print(alarmHours);
    lcd.print(": ");
    lcd.print(alarmMinites);
    lcd.print(": ");
    lcd.print(alarmSeconds);
    delay(80);
    return;
  }
  if ((Sec - initialSecond >= 1 || Time.second() == 0) && isSetAlarm == false) {
    initialSecond = Sec;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("current time");
    lcd.setCursor(0, 1);
    lcd.print(Hour);
    lcd.print(": ");
    lcd.print(Min);
    lcd.print(": ");
    lcd.print(Sec);
    Serial.println("updating screen");
    while ( Time.second() == 0) {
      Serial.println("waiting for the zero second to pass");
      Time = rtc.now();
    }
  }
}

void updateTimeVaribles() {
  Time = rtc.now();
  Hour = Time.hour();
  Min = Time.minute();
  Sec = Time.second();
}

void buzz(int waitTime = 50) {
  // make sound...
  digitalWrite(buzzer, true);
  delay(waitTime);
  digitalWrite(buzzer, false);
}

bool readBtn(int btn, int delayTime = 50) {
  // read and debounce button...
  if (digitalRead(btn) == true) return false;
  delay(delayTime);
  if (digitalRead(btn) == false) return true;
  else return false;
}

void setMode() {
  // set alarm mode...
  readBtn(configBtn, 20) && Serial.println("config button pressed");
  if (readBtn(configBtn, 50) == false || isAlarm == true) return;
  buzz();
  if (isSetAlarm == true && setAlarmState < 4) {
    ++setAlarmState;
    while (readBtn(configBtn, 20) == true);

  }
  if (setAlarmState >= 4 ) {
    setAlarmState = 0;
    isSetAlarm = false;
    Serial.println("setting set alarm to false");
    return;
  }
  if (isSetAlarm == false) {
    isSetAlarm = true;
    setAlarmState = 1;
    //check if the cingig button is still being pressed...
    while (readBtn(configBtn, 20)  == true);
    Serial.println("setting set alarm to true");
  }
  Serial.print("set alarm state: ");
  Serial.println(setAlarmState);


}

void setAlarmTime() {
  // set alarm time...
  if (isSetAlarm == false) return;

  if (setAlarmState == 1) {
    // set alarm hours...
    if (alarmHours < 24 && readBtn(rightBtn, 80)) {
      buzz();
      alarmHours ++;
    }
    if (alarmHours > 0 && readBtn(leftBtn, 80)) {
      buzz();
      alarmHours --;
    }
  } else if (setAlarmState == 2) {
    // set alarm minites...
    if (alarmMinites < 59 && readBtn(rightBtn, 80)) {
      alarmMinites ++;
      buzz();
    }
    if (alarmMinites > 0 && readBtn(leftBtn, 80)) {
      alarmMinites --;
      buzz();
    }
  } else if (setAlarmState == 3) {
    // set alarm seconds...
    if (alarmSeconds < 59 && readBtn(rightBtn, 80)) {
      alarmSeconds ++;
      buzz();
    }
    if (alarmSeconds > 0 && readBtn(leftBtn, 80)) {
      alarmSeconds --;
      buzz();
    }
  }
}

void setIsAlarm() {
  if ( (alarmHours == 0 && alarmMinites == 0 && alarmSeconds == 0)) return;
  if (Hour == alarmHours && Min == alarmMinites && Sec == alarmSeconds && isSetAlarm == false) isAlarm = true;
  if (readBtn(configBtn, 100) && isAlarm) {
    isAlarm = false;
    digitalWrite(buzzer, false);
    while (readBtn(configBtn, 20));
  }
}
