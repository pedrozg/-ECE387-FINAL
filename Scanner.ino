#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include <TimeAlarms.h>
boolean cancel=false;
int timeset=0;
int timecount=0;
boolean timeup;
boolean set=true;
boolean comfirm=false;
boolean leadboard=false;
boolean leadboardpage=false;
boolean printstat=false;
boolean pageback=false;
int numberinput=0;
int value=0;
int income = 0;
int addr = 0;
int pointer=0;
int compare=0;
int qrlength=0;
int flag=-1;
int matchflag=0;
int firstcount=0;
int timecursor=0;
int arraypointer=0;
boolean scanpage=true;
boolean scan=false;
const byte ROWS = 4; // number of rows
const byte COLS = 4; // number of columns
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {30, 32, 34, 36}; 
byte colPins[COLS] = {38, 40, 42, 44};    
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
    Serial.begin(9600);
	  lcd.begin();


	  lcd.backlight();
	 Serial3.begin(9600);
    qrlength=0;
    sizecheck();
}

void loop()
{  
  if(leadboardpage==true){
    getleadboard();
  }
  if(scanpage==true){
  lcd.setCursor(0,0);
  lcd.print("Log In");
  
      while (Serial3.available() > 0) {
      scan=true;
      income = Serial3.read();
      checking(income);      
       }
  if(scan==true&&income==13){
      unlock();
  } 
}
   

  if(set==false){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Set time ");
    
          while(numberinput<4){
      
          char key = keypad.getKey(); 
              if (key != NO_KEY){
                lcd.print(key);
                numberinput++;
              
                 if(numberinput==1){
                  value=(int)key-48;
  
                  timeset=timeset+value*600;
                 }
                  if(numberinput==2){
                      value=(int)key-48;
             
                   timeset=timeset+value*60;
                 }
                   if(numberinput==3){
                      value=(int)key-48;
       
                    timeset=timeset+value*10;
                 }
                   if(numberinput==4){
                      value=(int)key-48;
     
                   timeset=timeset+value;
                 }
              }
          }
         
          lcd.setCursor(0,1);
          lcd.print("Comfirm 'A'");
          while(comfirm==false){
              if(pageback==true){
                return;
              }
          char key = keypad.getKey(); 
              if (key != NO_KEY){
                  if(key=='A'){
                    comfirm=true;     
                  }
                    if(key=='D'){
                      arraypointer=0;
                      leadboardpage==true;
                      getleadboard();
                           
               }
              }
          }
          set==true;
          timeup=false;
          setTime(8,0,0,1,1,11);
            while(timeup==false){
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Time: ");
              digitalClockDisplay();
              Alarm.delay(1000); 
              timecount++;
  }
  timecount=0;
  }
  
  
}

void digitalClockDisplay() {

  lcd.setCursor(0,1);
  lcd.print(minute());
  lcd.print(":");
  lcd.print(second());
   lcd.setCursor(6,0);
   lcd.print("Cancel 'C'");
  printDigits(second());
  int temp=EEPROM.read(timecursor+2);
  temp++;
  if(temp>60){
    temp=temp-60;
    EEPROM.write(timecursor+2,temp);
    temp=EEPROM.read(timecursor+1)+1;
    EEPROM.write(timecursor+1,temp);
      if(temp>60){
         temp=temp-60;
         EEPROM.write(timecursor+1,temp);
         temp=EEPROM.read(timecursor)+1;
           EEPROM.write(timecursor,temp);
            }
  return;
  }
    EEPROM.write(timecursor+2,temp);
}

void printDigits(int digits) {
      char key = keypad.getKey(); 
              if (key != NO_KEY){
                  if(key=='C'){
                    cancel=true;
                    timeup=true;
                    set==false;
                    timeset=0;
                    numberinput=0;
   
                   comfirm=false;
                   delay(3000);
                    lcd.clear();
                       
                  }
              }
    if(timecount==timeset){
        lcd.clear();
        lcd.println("Times  up");
        timeup=true;
        set==false;
        timeset=0;
        numberinput=0;
   
   comfirm=false;
   delay(3000);
   lcd.clear();
  }     
     
}
void unlock(){
      if(matchflag!=qrlength){
      Serial.println("Not match");
      
      pointer=0;
      matchflag=0;
      
      scan=false;
      return ;
    }
    if(matchflag>=qrlength){
      Serial.println("LogIn");
      timecursor=pointer+1;
      set=false;
      scanpage=false;
      pointer=0;
      matchflag=0;
      scan=false;
      return;
    }
}
void sizecheck(){
  int i =-1;
  int arraypointer=0;
  while(i!=13){
    i=EEPROM.read(arraypointer);
   if(i!=0){
   qrlength++;
   arraypointer++;
   }
  }
}
void checking(int input){
  compare = EEPROM.read(pointer);
    if(compare==0){
      EEPROM.write(pointer,input);
      Serial.println(EEPROM.read(pointer));         
      pointer++;
      qrlength++;     
       firstcount++;
      
      return;
    }

    
    else if(input!=compare&&compare!=0){

      pointer++;
      return;
    }
   else  if(input==compare){
      
      pointer++;

     matchflag++;

    return;
    }
  }
void getleadboard(){
   

      lcd.clear();
      lcd.setCursor(0,0);
      int record=-1;
 
      record=EEPROM.read(arraypointer);
        while(record!=13){
          lcd.print((char)record);
          arraypointer++;
          record=EEPROM.read(arraypointer);
        }
  
        arraypointer=arraypointer+2;
        Serial.print(arraypointer);
        record=EEPROM.read(arraypointer);
        lcd.print(record);
        lcd.print(":");
        arraypointer++;
        record=EEPROM.read(arraypointer);
        lcd.print(record);
          arraypointer++;
          lcd.print(":");
          record=EEPROM.read(arraypointer);
          lcd.print(record);
      char key = keypad.getKey(); 
         if(key=='B'){
          pageback=true;
      }   
    
      
}
