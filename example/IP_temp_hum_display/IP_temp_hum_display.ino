/*
*Simple example for MAX6950
*input IP through serial port, end with '$', eg:"127.0.0.1$"
*IP will display on LED
*by Haotian Wang (04/09/2014)
*/

#include <MAX6950.h>
#include <SPI.h> //it must be included

char IP_buffer[50] = {'1','2','7','.','0','.','0','.','1','$'};
int cnt_IP_buffer = 0;
MAX6950 m;

void setup() {
  Serial.begin(115200);
  m.init();
  m.config_reg(0x11);
  m.intensity_reg(0x0f);
  m.decode_reg(0x00);
}

void loop(){
  Serial.println(IP_buffer);
  //m.blink(true);
  m.IP_display(IP_buffer);
  
  delay(1000);
  m.clearLED();
  delay(1000);
  //m.blink(false);
  m.temp_hum_display(27,34);
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
