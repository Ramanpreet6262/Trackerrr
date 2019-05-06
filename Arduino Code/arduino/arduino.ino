#include <SoftwareSerial.h>
SoftwareSerial gps(10,11); // RX, TX

char str[70];
String gpsString="";

char *test="$GPGGA";

String latitude="No Range";
String longitude="No Range";

int temp=0,i;
boolean gps_status=0;

void setup() 
{
  Serial.begin(9600);
  gps.begin(9600);
  Serial.print("**Vehicle Tracking System**");
  delay(2000);
  gsm_init();
  Serial.println("AT+CNMI=2,2,0,0,0");
  delay(2000);
  Serial.print("GPS Initializing");
  get_gps();
  delay(2000);
  temp=0;
}

void loop()
{
  serialEvent();
  if(temp)
  {
    get_gps();
    tracking();
  }
}

void serialEvent()
{
  while(Serial.available())
  {
    if(Serial.find("Track Vehicle"))
    {
      temp=1;
      break;
    }
    else
    temp=0;
  }
}

void gpsEvent()
{
  gpsString="";
  while(1)
  {
   while (gps.available()>0)            //checking serial data from GPS
   {
    char inChar = (char)gps.read();
     gpsString+= inChar;                    //store data from GPS into gpsString
     i++;
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //checking for $GPGGA sentence
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>65)
     {
       gps_status=1;
       break;
     }
     else
     {
       i=0;
     }
    }
  }
   if(gps_status)
    break;
  }
}

void gsm_init()
{
  Serial.print("Finding Module..");
  boolean at_flag=1;
  while(at_flag)
  {
    Serial.println("AT");
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
      at_flag=0;
    }
    
    delay(1000);
  }

  Serial.print("Module Connected..");
  delay(1000);
  Serial.print("Disabling ECHO");
  boolean echo_flag=1;
  while(echo_flag)
  {
    Serial.println("ATE0");
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
      echo_flag=0;
    }
    delay(1000);
  }

  Serial.print("Echo OFF");
  delay(1000);
  Serial.print("Finding Network..");
  boolean net_flag=1;
  while(net_flag)
  {
    Serial.println("AT+CPIN?");
    while(Serial.available()>0)
    {
      if(Serial.find("+CPIN: READY"))
      net_flag=0;
    }
    delay(1000);
  }
  Serial.print("Network Found..");
  delay(1000);
}

void get_gps()
{
   gps_status=0;
   int x=0;
   while(gps_status==0)
   {
    gpsEvent();
    int str_lenth=i;
    latitude="";
    longitude="";
    int comma=0;
    while(x<str_lenth)
    {
      if(gpsString[x]==',')
      comma++;
      if(comma==2)        //extract latitude from string
      latitude+=gpsString[x+1];     
      else if(comma==4)        //extract longitude from string
      longitude+=gpsString[x+1];
      x++;
    }
    int l1=latitude.length();
    latitude[l1-1]=' ';
    l1=longitude.length();
    longitude[l1-1]=' ';
    Serial.print("Lat:");
    Serial.print(latitude);
    Serial.print("Long:");
    Serial.print(longitude);
    i=0;x=0;
    str_lenth=0;
    delay(2000);
   }
}

void init_sms()
{
  Serial.println("AT+CMGF=1");
  delay(400);
  Serial.println("AT+CMGS=\"+9173555*****\"");   // use your 10 digit cell no. here
  delay(400);
}

void send_data(String message)
{
  Serial.println(message);
  delay(200);
}

void send_sms()
{
  Serial.write(26);
}

void tracking()
{
    init_sms();
    send_data("Your Vehicle current location is --");
    Serial.print("Latitude: ");
    send_data(latitude);
    Serial.print(", Longitude: ");
    send_data(longitude);
    send_data(". Please take some action soon..\nThankyou");
    send_sms();
    delay(2000);
}
