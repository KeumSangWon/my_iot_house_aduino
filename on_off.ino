#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Servo.h>

//servomotor
Servo myservo;
int pos = 0;

//WiFi
const char* ssid = "ummmm";
const char* password = "10203040";

WiFiUDP Udp;
unsigned int localUdpPort = 4211;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  replyPacket[] = "Hi there! Got the message :-)";  // a reply string to send back


void setup()
{
  Serial.begin(115200);
  Serial.println();

  //servomotor
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
  myservo.write(90);
  myservo.detach();

  //WiFi

  //Fixed IPAdress
  IPAddress ip(192,168,0,101);
  IPAddress gatway(192,168,0,1);
  IPAddress subnet(255,255,255,0);
  WiFi.config(ip, gatway, subnet);
    
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
 
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.println(incomingPacket);
    String value = incomingPacket;
    if(value == "on"){
      Serial.println("OK");
      
      //servomotor
      servo_ON();
     
      
      
    }else{
      servo_OFF();
    }

    // send back a reply, to the IP address and port we got the packet from
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(replyPacket);
    Udp.endPacket();
  }
}

void servo_ON(){
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
  myservo.write(140);
  delay(500);
  myservo.detach();
}

void servo_OFF(){
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object
  myservo.write(40);
  delay(500);
  myservo.detach();
}
