
#include <RCSwitch.h>
#include <ESP8266WiFi.h>

RCSwitch mySwitch = RCSwitch();
int port = 8888;  //Port number
WiFiServer server(port);

const char *ssid = "XXXX";  //Enter your wifi SSID
const char *password = "XXXX";  //Enter your wifi Password

const int STRING_BUFFER_SIZE = 50;
char stringBuffer[STRING_BUFFER_SIZE];
const char * startWord = "start";
const char * endWord = "stop";

bool readLine(Stream& stream)
{
  static int index;

  while(stream.available())
  {   
    char c = stream.read();
    
    if(c >= 32 && index < STRING_BUFFER_SIZE - 1)
    {
      stringBuffer[index++] = c;
    }
    else if(c == '\n' && index > 0)
    {
      stringBuffer[index] = '\0';
      index = 0;
      return true;
    }
  }
  return false;
}

void setup() {

  Serial.begin(9600);
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(D6);
  
  // set pulse length.
  mySwitch.setPulseLength(360);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi

  // Wait for connection  
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED)
  {   
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);
  
}

void loop()
{

  WiFiClient client = server.available();
  
  if (client)
  {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }
    
    while(client.connected())
    {      
     
        // read data from the connected client
        if(readLine(client))
        {
          Serial.println(stringBuffer);
          if(strcmp(stringBuffer, startWord) == 0)
          {
            Serial.println("Start sockets");
            client.println("Start sockets");
  
            mySwitch.switchOn('a', 1);
            mySwitch.switchOn('b', 1);
          }
          else if (strcmp(stringBuffer, endWord) == 0)
          {
            Serial.println("Stop sockets");
            client.println("Stop sockets");
  
            mySwitch.switchOff('a', 1);
            mySwitch.switchOff('b', 1);
          }
        
        }
    }
    client.stop();
    Serial.println("Client disconnected");    
  }  
}
