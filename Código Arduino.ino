#include "Stepper.h"
#include "IRremote.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define STEPS  32   // Number of steps per revolution of Internal shaft
#include "SR04.h"
#define TRIG_PIN 2
#define ECHO_PIN 3
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long a;

int  Steps2Take;  // 2048 = 1 Revolution
int receiver = 12; // Signal Pin of IR receiver to Arduino Digital Pin 6

LiquidCrystal_I2C lcd(0x27, 20, 4);
Stepper small_stepper(STEPS, 8, 10, 9, 11);
IRrecv irrecv(receiver);
decode_results results;

int sensor = 13; // DATO 1 SENSOR DE GAS
int gas_value;

void setup()


{
  irrecv.enableIRIn(); // Start the receiver
  lcd.begin();
  //lcd.backlight();
  lcd.setCursor(1, 0);
  pinMode(sensor, INPUT);

  Serial.begin(9600);
  delay(1000);
  
}

void loop()
{
  lcd.clear();
  gas_value=digitalRead(sensor);
  lcd.setCursor(2, 0);
  lcd.println(gas_value);
  

  
   a=sr04.Distance(); //DATO2 DISTANCIA VENTANA
   lcd.setCursor(3, 0);
   lcd.print(a);
   lcd.println("cm");
   
  delay(1000);
  if (irrecv.decode(&results)) // have we received an IR signal?

  {
    switch (results.value) // DATO DE ABRIR O CERRAR VENTANA (SI RECIBE 0xFFA857 CERAR Y 0xFF629D ABIERTO)

    {

      case 0xFFA857: // VOL+ button pressed DATO 3 ABRIR VNTANA
        lcd.clear();
        lcd.print("CLOSED");
        small_stepper.setSpeed(500); //Max seems to be 500
        Steps2Take  =  2048;  // Rotate CW
        small_stepper.step(Steps2Take);

        delay(4000);
        break;

      case 0xFF629D: // VOL- button pressed
        lcd.clear();
        lcd.print("OPEN");
        small_stepper.setSpeed(500);
        Steps2Take  =  -2048;  // Rotate CCW
        small_stepper.step(Steps2Take);

        delay(4000);
        break;

    }

    irrecv.resume(); // receive the next value
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
  }


}
