#include <SoftwareSerial.h>

//Serial Relay - Arduino will patch a 
//serial link between the computer and the GPRS Shield
//at 19200 bps 8-N-1
//Computer is connected to Hardware UART
//GPRS Shield is connected to the Software UART 
 

SoftwareSerial GPRS(2, 3);
unsigned char buffer[64]; // buffer array for data recieve over serial port
int count=0;     // counter for buffer array
boolean firstTimeInLoop = 1;
void setup()
{
  GPRS.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);             // the Serial port of Arduino baud rate.
  
 
}
 
void loop()
{
  if(firstTimeInLoop){
    GPRS.write("AT+CGATT=1");
    GPRS.write("\n\r");
    GPRS.write("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"");
    GPRS.write("\n\r");
    GPRS.write("AT+CDNSCFG=\"208.67.222.222\",\"208.67.220.220\"");
    GPRS.write("\n\r");
    GPRS.write("AT+CSTT=\"airtelgprs.com\",\"\",\"\"");
    GPRS.write("\n\r");
    GPRS.write("AT+CIICR");
    GPRS.write("\n\r");
    GPRS.write("AT+CIFSR");
    GPRS.write("\n\r");
    GPRS.write("AT+CIPSTATUS");
    GPRS.write("\n\r");
    GPRS.write("AT+CDNSORIP=1");
    GPRS.write("\n\r");
    GPRS.write("AT+CIPHEAD=1");
    GPRS.write("\n\r");
    GPRS.write("AT+CIPSTART=\"TCP\",\"www.nationalyouthparty.org\",\"80\"");
    GPRS.write("\n\r");
    GPRS.write("AT+CIPSEND");
    GPRS.write("\n\r");
    delay(2000);
    GPRS.write("GET http://www.nationalyouthparty.org/stick/mail.php HTTP/1.1^Z");
    GPRS.write("\n\r");
    firstTimeInLoop = 0;
  }
  
  if (GPRS.available())              // if date is comming from softwareserial port ==> data is comming from gprs shield
  {
    while(GPRS.available())          // reading data into char array 
    {
      buffer[count++]=GPRS.read();     // writing data into array
      if(count == 64)break;
  }
    Serial.write(buffer,count);            // if no data transmission ends, write buffer to hardware serial port
    clearBufferArray();              // call clearBufferArray function to clear the storaged data from the array
    count = 0;                       // set counter of while loop to zero
 
 
  }
  if (Serial.available())            // if data is available on hardwareserial port ==> data is comming from PC or notebook
    { 
      GPRS.write(Serial.read());
  }      // write it to the GPRS shield
}
void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<count;i++)
    { buffer[i]=NULL;}                  // clear all index of array with command NULL
}

