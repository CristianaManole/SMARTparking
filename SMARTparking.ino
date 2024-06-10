#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  //LCD
Servo myservo;  // creare un obiect servo

// definim pinii senzorilor IR
int IR1 = 2;
int IR2 = 3;

int Slot = 4;  // numarul de locuri de parcare

//pentru a urmari starea senzorilor IR
int flag1 = 0;
int flag2 = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();                  // initializam LCD
  lcd.backlight(); 

  pinMode(IR1, INPUT);         
  pinMode(IR2, INPUT);         

  myservo.attach(4);           
  myservo.write(100);          // pozitia initiala pentru servo

  // Display welcome message
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO OUR");
  lcd.setCursor(0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay(2000);
  lcd.clear();
}

void loop() {
  int readIR1 = digitalRead(IR1);
  int readIR2 = digitalRead(IR2);

  // verificare senzor de la intrare
  if (readIR1 == LOW) {
    if (flag1 == 0) {  // verificam daca este o masina noua(flag1 este 0)
      flag1 = 1;       // modificam stare flag1
      if (Slot > 0 && flag2 == 0) {  // verificam ca exista locuri de parcare goale si ca nu iese nicio masina 
        myservo.write(0);            // deschidem bariera
        Slot--;                      // scadem numarul locurilor de parcare
        delay(2000);                 // asteptam sa treaca masina
        myservo.write(100);          // inchidem bariera
      } else if (Slot <= 0) {
        lcd.setCursor(0, 0);
        lcd.print("    SORRY :(    ");
        lcd.setCursor(0, 1);
        lcd.print("  Parking Full  ");
        delay(3000);
        lcd.clear();
      }
    }
  } else {
    flag1 = 0;   
  }

  // verficare senzor de la iesire
  if (readIR2 == LOW) {
    if (flag2 == 0) {  //verificam daca iese o masina(flag2 este 0)
      flag2 = 1;       // modificam stare flag2
      if (flag1 == 0) {  // verificam ca nu intra nicio masina
        myservo.write(0);  // deschidem bariera
        Slot++;            // crestem numarul locurilor de parcare
        delay(2000);       // asteptam masina sa treaca
        myservo.write(100);  // inchidem bariera
      }
    }
  } else {
    flag2 = 0;  
  }

  // afisarea numarului locurilor de parcare ramase libere
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME!    ");
  lcd.setCursor(0, 1);
  lcd.print("Slot Left: ");
  lcd.print(Slot);
}