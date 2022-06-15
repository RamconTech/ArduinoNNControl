#include <LiquidCrystal_I2C.h>
#define echoPin 23
#define trigPin 22
#define solenoidPin 2
#define solenoidPin2 3
LiquidCrystal_I2C lcd(0x27, 16, 2);

float distance=0.0;
float duration=0.0;
int idistance=0;
String a="";
unsigned int Signal=1;
unsigned int prev_distance=0;

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.begin(16, 2);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  pinMode(solenoidPin2, OUTPUT);
  
  // initialize Timer1
  /*noInterrupts(); // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 3250; // compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= ((1 << CS12) | (0 << CS11) | (1 << CS10)); // 256 prescaler Set up Timer
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  interrupts(); // enable all interrupts*/
}

/*ISR(TIMER1_COMPA_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  //digitalWrite(trigPin, LOW);  // Added this line
  //delayMicroseconds(5); // Added this line
} */ 

void loop()
{ 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  //distance = duration/5.8;
  //distance =-(distance-210);
  //idistance=distance;
  distance=(duration*34/100)/2;
  distance = 230.0-distance;
  idistance=distance;  
  if(distance < 201)
  {
   Serial.write(idistance);
   Serial.print('\n');
   Serial1.print(idistance);
   Serial1.print('\n');
  }   
   lcd.setCursor(0, 0);
   lcd.print("Level ");
   lcd.print(distance);
   lcd.print(" mm");
  
  if (distance!=prev_distance)
  {
    delay(1000);
    lcd.clear(); 
  }
  
  prev_distance=distance;
  
  lcd.setCursor (0, 1);
  lcd.print("SerialRead ");
  if (Serial.available() > 0)
  {
      a=Serial.readString();
      Serial.setTimeout(100);
      Signal=a.toInt();
  }
  lcd.print(Signal);
  
  if (Signal==0)//pump on
  {    
    digitalWrite(solenoidPin, 0);
    digitalWrite(solenoidPin2, 1);
  }
  if (Signal==1)//pump off
  {
    digitalWrite(solenoidPin, 1);
    digitalWrite(solenoidPin2, 0);
  }
}


