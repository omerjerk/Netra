#include <SoftwareSerial.h>

//Serial Relay - Arduino will patch a 
//serial link between the computer and the GPRS Shield
//at 19200 bps 8-N-1
//Computer is connected to Hardware UART
//GPRS Shield is connected to the Software UART 
 
SoftwareSerial GPRS(10, 11);
unsigned char buffer[64]; // buffer array for data recieve over serial port
int count=0;     // counter for buffer array
char ctrlZ = 0x1A;
int inLoop;
void setup()
{
  GPRS.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600); 
}
 
void loop()
{
  if (GPRS.available())              // if date is comming from softwareserial port ==> data is comming from gprs module
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
   
   GPRS.write("AT+CMGF=1");
  GPRS.write("\n\r"); 
  if(inLoop == 3){
    GPRS.write("AT+CMGS=\"**********\"");  // Phone number
    GPRS.write("\n\r");
    delay(30);
    GPRS.write("Hello World"); // Message
    GPRS.write("\n\r");
    GPRS.write("\n");
    GPRS.write(ctrlZ);
    GPRS.write("\n\r");
    Serial.println("Sending message!");
  }
  
  ++inLoop;
}

void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<count;i++)
    { buffer[i]=NULL;}                  // clear all index of array with command NULL
}
