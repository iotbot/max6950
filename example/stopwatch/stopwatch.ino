

/*
A simple exampe of stopwatch using MAX6950
By Haotian Wang (04/08/2014)
*/


#include <MAX6950.h>
#include <SPI.h> //it must be included

MAX6950 m;
int dig_0 = 0;
int dig_1 = 0;
int dig_2 = 0;
int dig_3 = 0;
int millisencond = 0;

char* s= "10.238.154.81\n";
char* s1= "255.255.255.0\n";
char IP_buffer[20];
int cnt_IP_buffer = 0;

void setup() {
  //MAX6950 initialization
  m.init();
  m.config_reg(0x19);
  m.intensity_reg(0x0f);
  m.decode_reg(0xff);
  Serial.begin(115200);
}

void loop(){
  /*m.IP_print(s);
  delay(1000);
  m.clearLED(1);
  delay(1000);
  m.clearLED(0);
  m.IP_print(s1);
  delay(1000);*/

  Serial.print(IP_buffer);
  m.IP_print(IP_buffer);
  cnt_IP_buffer = 0;
  
  delay(1000);
  m.clearLED(1);
  m.IP_print(s1);
  
  delay(1000);
  m.clearLED(0);
  delay(1000);
  
  
}

void read_IP_buffer(void)
{
  cnt_IP_buffer = 0;
  while(Serial.available() > 0)
  {
      IP_buffer[cnt_IP_buffer]= char(Serial.read());
      delay(2);
      cnt_IP_buffer++;
  }
  IP_buffer[cnt_IP_buffer] = '\n';
  
  
}

void serialEvent(){  
  while(Serial.available()){
    char inChar = (char)Serial.read();
    IP_buffer[cnt_IP_buffer] = inChar;
    cnt_IP_buffer++;
   }
}

char* print_string(char* str)
{
  int cnt = 0;
  int cnt1 = 0;
  char str1[20];
  char* tmp = str;
  while(tmp[cnt1] != '\n')
  {
    if(tmp[cnt1] == 0x2e)
      str1[cnt-1] = str1[cnt-1] | 0x80;
    else
    {
       str1[cnt] = tmp[cnt1] - '0';
       cnt++;
    }  
    cnt1++;
  }
  str1[cnt] = '$';
  //Serial.print(str1);
  int i = 0;
  while(str1[i + 3] != '$')
  {
    m.set_digital(0x3,str1[i]);
    m.set_digital(0x2,str1[i+1]);
    m.set_digital(0x1,str1[i+2]);
    m.set_digital(0x0,str1[i+3]);
    i++;
    delay(1000); 
  }
  
}

