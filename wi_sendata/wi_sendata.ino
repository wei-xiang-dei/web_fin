#include <WiFi.h>
#include <WiFiUdp.h>
#include "MPU9250.h"
/* WiFi network name and password */
MPU9250 mpu;
const char * ssid = "Ven_peap";
const char * pwd = "";

// IP address to send UDP data to.
// it can be ip address of the server or 
// a network broadcast address
// here is broadcast address
const char * udpAddress = "192.168.139.150";
const int udpPort = 44444;
int buttonState = 0;
int diffbutton = 0;
//create UDP instance
WiFiUDP udp;

void setup(){
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
  Serial.begin(19200);
  Wire.begin();
  delay(2000);

    if (!mpu.setup(0x68)) {  // change to your own address
        while (1) {
            Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
            delay(5000);
        }
    }
//    Serial.println("Accel Gyro calibration will start in 5sec.");
//    Serial.println("Please leave the device still on the flat plane.");
    mpu.verbose(true);
    delay(5000);
    mpu.calibrateAccelGyro();

//    Serial.println("Mag calibration will start in 5sec.");
//    Serial.println("Please Wave device in a figure eight until done.");
//    delay(5000);
//    mpu.calibrateMag();
//  
//    print_calibration();
//    mpu.verbose(false);
    mpu.setFilterIterations(15);
    pinMode(25,INPUT_PULLUP);
    int i = 0;
    while(i < 50 ){
       if (mpu.update()) {
       static uint32_t prev_ms = millis();
       if (millis() > prev_ms + 40) {
          uint8_t buffer[50] = "";
          char buf2 [60]="";
          //This initializes udp and transfer buffer
          sprintf(buf2,"%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",mpu.getGyroX(),mpu.getGyroY(),mpu.getGyroZ(),mpu.getAccX(),mpu.getAccY(),mpu.getAccZ(),mpu.getMagX(),mpu.getMagY(),mpu.getMagZ());
//          Serial.println(strlen(buf2));
          memcpy(buffer,buf2,strlen(buf2));
          udp.beginPacket(udpAddress, udpPort);
          udp.write(buffer,strlen(buf2));
          udp.endPacket();
//           getgyro();
//           acc();
//           //linearacc();
//           Mag();
           prev_ms = millis();
           i = i +1;
       }
       }
       
      }
  
 
}

void loop(){

  buttonState = digitalRead(25);
  if(buttonState != diffbutton){
    int i = 0;
    uint8_t buffer[50][50] = "";
    char buf2 [50][60]="";
    while(i < 50){
      if (mpu.update()) {
          
          //This initializes udp and transfer buffer
          sprintf(buf2[i],"%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",mpu.getGyroX(),mpu.getGyroY(),mpu.getGyroZ(),mpu.getAccX(),mpu.getAccY(),mpu.getAccZ(),mpu.getMagX(),mpu.getMagY(),mpu.getMagZ());
          Serial.println(strlen(buf2[i]));
          memcpy(buffer[i],buf2[i],strlen(buf2[i]));
          udp.beginPacket(udpAddress, udpPort);
          udp.write(buffer,strlen(buf2[i]));
          udp.endPacket();
          memset(buffer[i], 0, 50);
        static uint32_t prev_ms = millis();
        if (millis() > prev_ms + 50) {
         getgyro();
         acc();
         //linearacc();
         Mag();
         prev_ms = millis();
         i = i +1;
       
      }
   }
   
   
      }
   for(int a=0;a<50;a++){
    for(int z=0;z<10;z++){
      udp.beginPacket(udpAddress, udpPort);
      udp.write(buffer[a],strlen(buf2[a]));
      udp.endPacket();
    }
    
   }
    
   diffbutton = diffbutton^(1);
    }
 else{
  uint8_t buf3[5]="off\n";
  udp.beginPacket(udpAddress, udpPort);
  udp.write(buf3,3);
  udp.endPacket();
  Serial.println("off");
  
  }
}



void print_roll_pitch_yaw() {
    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getYaw(), 2);
    Serial.print(", ");
    Serial.print(mpu.getPitch(), 2);
    Serial.print(", ");
    Serial.println(mpu.getRoll(), 2);
}

void getgyro() {
  Serial.print(mpu.getGyroX(), 2);
  Serial.print(",");
  Serial.print(mpu.getGyroY(), 2);
  Serial.print(",");
  Serial.print(mpu.getGyroZ(), 2);
  Serial.print(",");
  }
void acc(){
  Serial.print(mpu.getAccX(), 2);
  Serial.print(",");
  Serial.print(mpu.getAccY(), 2);
  Serial.print(",");
  Serial.print(mpu.getAccZ(), 2);
  Serial.print(",");
  }
void Mag() {
  Serial.print(mpu.getMagX(), 2);
  Serial.print(",");
  Serial.print(mpu.getMagY(), 2);
  Serial.print(",");
  Serial.println(mpu.getMagZ(), 2);
  }
void linearacc() {
  Serial.print(mpu.getLinearAccX(),2);
  Serial.print(",");
  Serial.print(mpu.getLinearAccY(),2);
  Serial.print(",");
  Serial.print(mpu.getLinearAccZ(),2);
  Serial.print(",");
  }
void print_calibration() {
    Serial.println("< calibration parameters >");
    Serial.println("accel bias [g]: ");
    Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
    Serial.println();
    Serial.println("gyro bias [deg/s]: ");
    Serial.print(mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.print(", ");
    Serial.print(mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
    Serial.println();
    Serial.println("mag bias [mG]: ");
    Serial.print(mpu.getMagBiasX());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasY());
    Serial.print(", ");
    Serial.print(mpu.getMagBiasZ());
    Serial.println();
    Serial.println("mag scale []: ");
    Serial.print(mpu.getMagScaleX());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleY());
    Serial.print(", ");
    Serial.print(mpu.getMagScaleZ());
    Serial.println();
}
