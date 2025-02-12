#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define USE_UI_CONTROL 0

#if USE_UI_CONTROL
#include <MD_UISwitch.h>
#endif

#define DEBUG 0

#if DEBUG
#define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTX(x) Serial.println(x, HEX)
#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTX(x)
#endif

#define MAX_DEVICES 8
#define CLK_PIN   13    // Clock Pulse Source
#define DATA_PIN  11    // Data In
#define CS_PIN    10    // Chip Select

// HARDWARE SPI
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// SOFTWARE SPI
//MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Scrolling parameters
//#if USE_UI_CONTROL
//const uint8_t SPEED_IN = A5;
//const uint8_t DIRECTION_SET = 8;  // change the effect
//const uint8_t INVERT_SET = 9;     // change the invert
//
//const uint8_t SPEED_DEADBAND = 5;
//#endif // USE_UI_CONTROL

uint8_t scrollSpeed = 60;
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
uint16_t scrollPause = 50;

#define	BUF_SIZE	75
char data = 0;
char curMessage[BUF_SIZE] = { "\n" };
char newMessage[BUF_SIZE] = { data };
bool newMessageAvailable = true;

//#if USE_UI_CONTROL
//
//MD_UISwitch_Digital uiDirection(DIRECTION_SET);
//MD_UISwitch_Digital uiInvert(INVERT_SET);

//void doUI(void)
//{
//  // set the speed if it has changed
//  {
//    int16_t speed = map(analogRead(SPEED_IN), 0, 1023, 10, 150);
//
//    if ((speed >= ((int16_t)P.getSpeed() + SPEED_DEADBAND)) ||
//      (speed <= ((int16_t)P.getSpeed() - SPEED_DEADBAND)))
//    {
//      P.setSpeed(speed);
//      scrollSpeed = speed;
//      PRINT("\nChanged speed to ", P.getSpeed());
//    }
//  }
//
//  if (uiDirection.read() == MD_UISwitch::KEY_PRESS) // SCROLL DIRECTION
//  {
//    PRINTS("\nChanging scroll direction");
//    scrollEffect = (scrollEffect == PA_SCROLL_LEFT ? PA_SCROLL_RIGHT : PA_SCROLL_LEFT);
//    P.setTextEffect(scrollEffect, scrollEffect);
//    P.displayClear();
//    P.displayReset();
//  }
//
//  if (uiInvert.read() == MD_UISwitch::KEY_PRESS)  // INVERT MODE
//  {
//    PRINTS("\nChanging invert mode");
//    P.setInvert(!P.getInvert());
//  }
//}
//#endif // USE_UI_CONTROL

void readSerial(void)
{
  static char *cp = newMessage;

  while (Serial.available())
  {
    *cp = (char)Serial.read();
    if ((*cp == '\n') || (cp - newMessage >= BUF_SIZE-2))
    {
      *cp = '\0'; // akhir dari string
      // mulai ulang indeks untuk pengisian dan bendera berikutnya, kami memiliki pesan yang menunggu
      cp = newMessage;
      newMessageAvailable = true;
    }
    else  // memindahkan monitor char ke posisi berikutnya
      cp++;
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.print(data, "\n");
  P.begin();
  P.displayText(newMessage, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
}

void loop()
{
    Running();
}

void Running(void) {
if (P.displayAnimate())
{
  if (newMessageAvailable)
  {
    strcpy("" ,newMessage);
    newMessageAvailable = true;
  }
  P.displayReset();
}
readSerial();
}

