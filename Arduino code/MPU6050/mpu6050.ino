#include<Wire.h>
#include <SoftwareSerial.h>
#include <math.h>

const int MPU6050_addr = 0x68; // I2C address of the MPU-6050
int16_t AccX, AccY, AccZ;
float y_val;
int flag;
int timer;

char str_x[20];
char str_y[20];
char str_z[20];

SoftwareSerial BTserial(10, 11); // RX | TX

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr); // start MPU6050 transmission
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  BTserial.begin(115200);
  Serial.begin(115200);
  flag = 0;
  timer = 0;
}
void loop() {
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H) 即 ACC_X 的地址
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr, 6, true); // request a total of 14 registers
  AccX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AccY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AccZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  y_val = (AccY - 43.28) / 16384.0;

  dtostrf((AccX - 1603.57) / 16384.0,2,6,str_x);
  dtostrf(y_val,2,6,str_y);
  dtostrf((AccZ + 338.55) / 16384.0,2,6,str_z);
  
  if ( abs(y_val-1) >= 0.1 ){ // 桌面震动
    timer = 0;
    flag = 1;
    // ref: http://www.lab-z.com/floatexp/
   
    Serial.println(String(str_x) + "," + String(str_y) + "," + String(str_z));
    BTserial.print(String(str_x) + "," + String(str_y) + "," + String(str_z) + ";");
  }else{
    if (flag == 1){
      timer += 1; 
      if (timer >  50){
        BTserial.print("!");
        Serial.print("!");
        flag = 0;
      }else{
        Serial.println(String(str_x) + "," + String(str_y) + "," + String(str_z));
        BTserial.print(String(str_x) + "," + String(str_y) + "," + String(str_z) + ";");
      }
    }
  }
  
  delay(1);
}
