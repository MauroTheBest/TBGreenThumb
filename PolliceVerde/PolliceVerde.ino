#include <SimpleDHT.h>
#include <U8glib.h>

int mostureSensor = 0;
int pinDHT11 = 3;
int pinLight = A3;
int pinButton = 7;
int LCDStatus = HIGH;
int lastButtonState = HIGH; 
int buttonState;
SimpleDHT11 dht11;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);

long previousMillis = 0; 
long interval = 5000;    

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode( pinLight, INPUT);
  pinMode( pinButton, INPUT);
}

void loop() {

  unsigned long currentMillis = millis();
  int reading = digitalRead(pinButton);
  if (reading == HIGH) {
    lastButtonState = HIGH;
  }else{
    lastButtonState = LOW;
  }
  
  
  Serial.print("LCD: ");
  Serial.println(LCDStatus);
  
   Serial.print("Button: ");
   Serial.println(lastButtonState);
   Serial.println("");


    if (lastButtonState == HIGH &&digitalRead(pinButton) == LOW) {

      if (LCDStatus == HIGH){
        u8g.sleepOff();
        LCDStatus = LOW;
        //delay(2000);
      }else{
        u8g.sleepOn();
        LCDStatus = HIGH;
        
      }
    }

    
  if(currentMillis - previousMillis > interval ) {
    // read with raw sample data.
    byte temperature = 0;
    byte humidity = 0;
    byte data[40] = {0};
    String soil = "";
    String light = "";
    
  
    int sensorValue = analogRead(mostureSensor);
    //Serial.println(sensorValue);
  
    if ( sensorValue >= 800){
      soil = "DRY";
    }else if( sensorValue <= 799 and sensorValue >=500 ){
      soil = "HUMID";
    }else{
      soil = "WET";
    }
  
    if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
      Serial.print("Read DHT11 failed");
      return;
    }
    //Light sensor
     int LightValue = analogRead(pinLight);
    //Serial.println((int)temperature); Serial.print(" *C, ");
    //Serial.print((int)humidity); Serial.println(" %");
    
    if ( LightValue >= 300){
      light = "ON";
    }else{
      light = "OFF";
    }
  
    u8g.firstPage();  
    do {
      u8g.setFont(u8g_font_unifont);
      //u8g.setRot180();
      u8g.drawStr( 0, 15, "TEMP: ");
      u8g.setPrintPos(75, 15); 
      u8g.print(temperature);
      u8g.drawStr( 95, 15, "\260C");
      u8g.drawStr( 0, 30, "HUMIDITY: ");
      u8g.setPrintPos(75, 30); 
      u8g.print(humidity);
      u8g.drawStr( 90, 30, "%");
      u8g.drawStr( 0, 45, "LIGHT: ");
      u8g.setPrintPos(75, 45); 
      u8g.print(light);
      u8g.drawStr( 0, 60, "SOIL: ");
      u8g.setPrintPos(75, 60); 
      u8g.print(soil);
  
    } while( u8g.nextPage() );


    previousMillis = currentMillis;  
  } 
  
 // delay(1000);  
}
