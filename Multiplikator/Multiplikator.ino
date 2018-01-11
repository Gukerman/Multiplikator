/*
 * Кабельный мультипликатор
*/

#include <LiquidCrystal.h>                // Подключяем библиотеку
LiquidCrystal lcd( 8, 9, 4, 5, 6, 7 );    // Указываем порты         

//Определяем переменные длительность импульса и задержка
  int d=100;  // пауза в микросекундах между импульсами на мотор
  bool revers;
    
//Определяем порты подключенные к ШД
//int PUL=8; //define Pulse pin
//int DIR=9; //define Direction pin
//int ENA=10; //define Enable Pin

// Первый станок
 int PUL=3; //define Pulse pin
 int DIR=12; //define Direction pin
 int ENA=13; //define Enable Pin

//Определяем порты подключенные к концевикам
int port_l = 58;  //А4
int port_r = 59;  //А5
int port_g = 57;  //А3

//int k=0;    // переменная для задержки нажатия кнопок
int x;                                  // Создаем переменную x опрос кнопок экрана
int key_l=0;    //концевик слева
int key_r=0;    //концевик справв
int gerkon = 0;   
bool flag = true;

int drive = 20;    //438*2;    //количество шагов на один проход
long dreb = 0;    // переменная для хранения времени опроса кнопок в миллисекундах
long dreb_g = 0;    // переменная для хранения времени опроса геркона в миллисекундах
long pause = 0;    // переменная для хранения времени опроса геркона в миллисекундах

double mm = 50;    // диаметр в мм
int s_mm = 0;   // количество проходов mm каждый по 1 мм

int summ=0;
long mil_summ=0;

void setup()
{
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);
  
  pinMode(port_l,INPUT);
  pinMode(port_r,INPUT);
  pinMode(port_g,INPUT);

  lcd.begin(16, 2);                       // Инициализируем LCD 16x2  
  lcd.setCursor(0,0);                     // Установить курсор на первую строку  
  lcd.print("U-Multiplikator");                   // Вывести текст
  lcd.setCursor(0,1);                     // Установить курсор на вторую строку
  lcd.print("Diametr");              // Вывести текст
  lcd.setCursor(9,1);                    // Установить курсор на конец строки
  lcd.print(mm/10);                        // Вывести скорость
  lcd.print("mm");                        // Вывести скорость
  Serial.begin(9600);                     // Включаем последовательный порт

}
 
void loop() { 

  // Крутить двигатель 
if ((digitalRead(port_g)== HIGH )&&( 50 < (millis()-dreb_g) ))   flag=true;
if ((digitalRead(port_g)== LOW ) && (flag)) 
{ 
  gerkon = 1;
  flag = false;
  dreb_g=millis();
  s_mm = 0;
  
  //Serial.print(summ++);
  //Serial.print(' - ');
  
  //Serial.println((millis() - mil_summ));
  //mil_summ = millis();
  
}

if ((s_mm < mm)&&(gerkon == 1)) 
  
  {
  if (revers) back();
  else forward();
  s_mm ++;
//    Serial.println(s_mm); 
  }
else 
  {
  if ( 333 < (millis()-dreb_g))   
    {
    s_mm = 0;
    gerkon = 0;
    }
  }

// gerkon = 1;

if (digitalRead(port_l)==LOW) 
  {
    revers=1;
//      Serial.println("Left"); 
  }
if (digitalRead(port_r)==LOW) 
  {
    revers=0;
      //Serial.println("Rihgt"); 
  }

  x=analogRead (0);
 
  if ( 200 < millis()-dreb )                    
       {
        lcd.setCursor(9,1);                    // Установить курсор на вторую строку 
        if (x < 90)  
        {
        if (digitalRead(port_l)==HIGH)    forward();                        // Разворот      
//        Serial.println("Left"); 
        }
        
        else if (x < 200) {                     // +
        mm++;
        lcd.setCursor(9,1);                    // Установить курсор на конец строки
        lcd.print("             ");              // Очистка экрана
        lcd.setCursor(9,1);                    // Установить курсор на конец строки
        lcd.print(mm/10);                        // Вывести скорость
        lcd.print("mm");                        // Вывести скорость
        dreb = millis();
      }
        else if (x < 400){                      // -
        if (mm >= 0.1) mm--;
        lcd.setCursor(9,1);                    // Установить курсор на конец строки
        lcd.print("             ");              // Очистка экрана
        lcd.setCursor(9,1);                    // Установить курсор на конец строки
        lcd.print(mm/10);                        // Вывести скорость
        lcd.print("mm");                        // Вывести скорость
        dreb = millis();
      }
        else if (x < 600)
          {
          if (digitalRead(port_r)==HIGH) back();       
        //  Serial.println("Rihgt");
          }                   // Если х меньше 600 перейти на следующию строку
            else if ((x < 800) || ( 30000 < millis()-pause)){                      // Если х меньше 800 перейти на следующию строку
            for (int i=0; i<1000000; i++)    //Forward пока не конец
              {
//            Serial.println(i);                  // Вывести значение переменной x
            if (digitalRead(port_l)==HIGH)  forward();
            else 
            {
              Serial.println("break");                  // Вывести значение переменной x
              break;
              pause = millis();
            }
              }
            }
        }
}


void forward()
{
    digitalWrite(ENA,LOW);
  for (int i=0; i<drive; i++)    //Forward 5000 steps 1 mm
  {
    digitalWrite(DIR,LOW);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(d);
    digitalWrite(PUL,LOW);
    delayMicroseconds(d);
  }

    digitalWrite(ENA,HIGH);
    pause = millis();
}

void back()
{
    digitalWrite(ENA,LOW);

  for (int i=0; i<drive; i++)   //Backward 5000 steps 1 mm
  {
    digitalWrite(DIR,HIGH);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(d);
    digitalWrite(PUL,LOW);
    delayMicroseconds(d);
  }

    digitalWrite(ENA,HIGH);
    pause = millis();
}
