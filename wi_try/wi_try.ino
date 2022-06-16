#include <WiFi.h>
#include <WiFiUdp.h>

/* WiFi network name and password */
const char * ssid = "Ven_peap";
const char * pwd = "";

// IP address to send UDP data to.
// it can be ip address of the server or 
// a network broadcast address
// here is broadcast address
const char * udpAddress = " 192.168.139.17";
const int udpPort = 44444;

//create UDP instance
WiFiUDP udp;

void setup(){
  Serial.begin(115200);
  
  //Connect to the WiFi network
   WiFi.begin(ssid, pwd);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

float a=0.015;
float b=0.075;
float c=0.155;

void loop(){
  //data will be sent to server
  
  uint8_t buffer[60]="you are -0.5647 and, bebe"; 
  char buf2 [60];
  sprintf(buf2,"a is %f ,b is %f, c is %f",a,b,c);
  //This initializes udp and transfer buffer
  memcpy(buffer,buf2,60);
  udp.beginPacket(udpAddress, udpPort);
  udp.write(buffer,60);
  udp.endPacket();
  memset(buffer, 0, 50);
  //processing incoming packet, must be called before reading the buffer
  udp.parsePacket();
  //receive response from server, it will be HELLO WORLD
  if(udp.read(buffer, 50) > 0){
    Serial.print("Server to client: ");
    Serial.println((char *)buffer);
  }
  //Wait for 1 second
  delay(1000);
}
