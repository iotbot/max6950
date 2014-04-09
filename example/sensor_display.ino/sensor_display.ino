/* 
 * //////////////////////////////////////////////////
 * //making sense of the Parallax PIR sensor's output
 * //////////////////////////////////////////////////
 *
 * Switches a LED according to the state of the sensors output pin.
 * Determines the beginning and end of continuous motion sequences.
 *
 * @author: Kristian Gohlke / krigoo (_) gmail (_) com / http://krx.at
 * @date:   3. September 2006 
 *
 * kr1 (cleft) 2006 
 * released under a creative commons "Attribution-NonCommercial-ShareAlike 2.0" license
 * http://creativecommons.org/licenses/by-nc-sa/2.0/de/
 *
 *
 * The Parallax PIR Sensor is an easy to use digital infrared motion sensor module. 
 * (http://www.parallax.com/detail.asp?product_id=555-28027)
 *
 * The sensor's output pin goes to HIGH if motion is present.
 * However, even if motion is present it goes to LOW from time to time, 
 * which might give the impression no motion is present. 
 * This program deals with this issue by ignoring LOW-phases shorter than a given time, 
 * assuming continuous motion is present during these phases.
 *  
 */

/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
#include <dht.h>
#include <MAX6950.h>
#include <SPI.h> //it must be included

int calibrationTime = 1;        
dht DHT;
char IP_buffer[50] = {'1','2','7','.','0','.','0','.','1','$'};
int cnt_IP_buffer = 0;
MAX6950 m;

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  

// output DHT11 every 1s
int dht11_last = 0, dht11_now = 0;
int temperature_interval = 3000; // 3s interval reporting

int pir_last = 0, pir_now = 0;
// 30s interval for PIR detection
// Note: On the Arduino Uno (and other ATMega based boards) an int stores a 16-bit (2-byte) value. 
// This yields a range of -32,768 to 32,767 (minimum value of -2^15 and a maximum value 
// of (2^15) - 1). 
int PIR_interval = 30000;
int pir_reported = 0;

int pirPin = 3;    //the digital pin connected to the PIR sensor's output
int ledPin = 13;


/////////////////////////////
//SETUP
void setup(){
  Serial.begin(115200);
  m.init();
  m.config_reg(0x11);
  m.intensity_reg(0x0f);
  m.decode_reg(0x00);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);
  pinMode(2,INPUT);
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

////////////////////////////
//LOOP
void loop(){
  dht11_now = millis();
  if (dht11_now - dht11_last > temperature_interval) {
    int chk = DHT.read11(2);
    switch (chk)
    {
    case 0:  
      Serial.print("{\"DHT11_humidity\":");
      Serial.print(DHT.humidity,1);
      Serial.print(",\"DHT11_temperature\":");
      Serial.print(DHT.temperature,1);
      Serial.print("}\n");
      
      break;
    case -1: 
      Serial.print("Checksum error,\t"); 
      break;
    case -2: 
      Serial.print("Time out error,\t"); 
      break;
    default: 
      Serial.print("Unknown error,\t"); 
      break;
    }
    dht11_last = millis();
  }

  if(digitalRead(pirPin) == HIGH){
    // detected motion
    digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
    
    if (!pir_reported) { 
      Serial.print("{\"PIR\":1}\n");
      pir_reported = 1;
      pir_last = millis();
    }
  }
  
  pir_now = millis();
  if (pir_now - pir_last > PIR_interval) {
    pir_reported = 0;
  }
  delay(100);  
  
  Serial.println(IP_buffer);
  m.IP_display(IP_buffer);
  m.clearLED();
  m.temp_hum_display(int(DHT.temperature),int(DHT.humidity));
}

void serialEvent(){
  while(Serial.available()){
    char inChar = (char)Serial.read();
    IP_buffer[cnt_IP_buffer] = inChar;
    cnt_IP_buffer++; 
   }
   IP_buffer[cnt_IP_buffer] = '\o';
   cnt_IP_buffer = 0;
}
