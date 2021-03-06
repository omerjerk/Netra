#include <SoftwareSerial.h>

//Serial Relay - Arduino will patch a 
//serial link between the computer and the GPRS Shield
//at 19200 bps 8-N-1
//Computer is connected to Hardware UART
//GPRS Shield is connected to the Software UART 
 
SoftwareSerial GPRS(2, 3);
unsigned char buffer[64]; // buffer array for data recieve over serial port
int count=0;     // counter for buffer array
boolean inLoop = 0;
char ctrlZ = 0x1A;
float latitude , longitude;
void setup()
{
  GPRS.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);
 
}
 
void loop()
{
  do{
    GPRS.write("AT+CGATT=1"); //Attach a GPRS Service
    GPRS.write("\n\r");
    GPRS.write("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"");  //Define PDP Context
    GPRS.write("\n\r");
    GPRS.write("AT+CDNSCFG=\"208.67.222.222\",\"208.67.220.220\"");   //Configure Domain Name Server
    GPRS.write("\n\r");
    GPRS.write("AT+CSTT=\"airtelgprs.com\",\"\",\"\""); //Start Task & set APN, User ID, and password
    GPRS.write("\n\r");
    GPRS.write("AT+CIICR");   //Bring up wireless connection with GPRS  P.S.  Time consuming
    GPRS.write("\n\r");
    GPRS.write("AT+CIFSR");   // Get Local IP address 
    GPRS.write("\n\r");
    GPRS.write("AT+CIPSTATUS");  // Get Connection Status P.S.  It should be 'IP STATUS'
    GPRS.write("\n\r");
    GPRS.write("AT+CIPHEAD=1");   // Add headers to the PHP request. 
    GPRS.write("\n\r");
    GPRS.write("AT+CDNSORIP=1");  //Indicates whether connection request will be IP address (0), or domain name (1)
    GPRS.write("\n\r");
    GPRS.write("AT+CIPSTART=\"TCP\",\"www.ludlowcastle.co.in\",\"80\"");  //Start up TCP connection (mode, IP address/name, port) P.S. if returns 'CONNECT OK' then you're lucky
    GPRS.write("\n\r");
    
    ++inLoop;
    delay(1000);
  } while (inLoop < 4);
  
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
  
  updateLocation(23.6780,34.3458);
}
void clearBufferArray()              // function to clear buffer array
{
  for (int i=0; i<count;i++)
    { buffer[i]=NULL;}                  // clear all index of array with command NULL
}

void updateLocation(float lat, float lon){
    delay(7000);
    GPRS.write("AT+CIPSEND");  // Send the Data
    GPRS.write("\n\r");
    Serial.print("\nSending Location Data !!\n");
    GPRS.write("GET /stick/updateLoc.php HTTP/1.1");   //It's the actual HTTP request finally
    GPRS.write("\n");
    GPRS.write("Host: www.ludlowcastle.co.in");
    GPRS.write("\n");
    GPRS.write("Connection: keep-alive");
    GPRS.write("\n");
    GPRS.write("Accept: */");
    GPRS.write("*");
    GPRS.write("\n");
    GPRS.write("Accept-Language: en-us");
    GPRS.write("\n\r");
    GPRS.write("\n");
    GPRS.write(ctrlZ);
    GPRS.write("\n\r");
}

