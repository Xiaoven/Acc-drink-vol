#include<Wire.h>
const int MPU6050_addr = 0x68; // I2C address of the MPU-6050
float AccX, AccY, AccZ, Temp, GyroX, GyroY, GyroZ;
float AccXDev, AccYDev, AccZDev, GyroXDev, GyroYDev, GyroZDev;
int c = 0;
void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU6050_addr); // start MPU6050 transmission
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(115200);

  cal_deviation();
  delay(50);
}
void loop() {
  Wire.beginTransmission(MPU6050_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H) 即 ACC_X 的地址
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_addr, 14, true); // request a total of 14 registers
  AccX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AccY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AccZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Temp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyroX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyroY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyroZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.print("AccX = "); Serial.print(AccX);
  Serial.print(" || AccY = "); Serial.print(AccY);
  Serial.print(" || AccZ = "); Serial.print(AccZ);
  Serial.print(" || GyroX = "); Serial.print(GyroX);
  Serial.print(" || GyroY = "); Serial.print(GyroY);
  Serial.print(" || GyroZ = "); Serial.print(GyroZ);
  Serial.print(" || Temp = "); Serial.println(Temp / 340.00 + 36.53); //equation for temperature in degrees C from datasheet
  delay(500);
}

void cal_deviation(){
  while (c < 2000){
    Wire.beginTransmission(MPU6050_addr);
    Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H) 即 ACC_X 的地址
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_addr, 14, true); // request a total of 14 registers
    AccX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AccY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AccZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Temp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyroX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyroY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyroZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

    AccXDev = AccXDev + AccX;
    AccYDev = AccYDev + AccY;
    AccZDev = AccZDev + AccZ;
    GyroXDev = GyroXDev + GyroX;
    GyroYDev = GyroYDev + GyroY;
    GyroZDev = GyroZDev + GyroZ;

    c = c+1;
  }

  AccXDev = AccXDev / 2000;
  AccYDev = AccYDev / 2000;
  AccZDev = AccZDev / 2000;
  GyroXDev = GyroXDev / 2000;
  GyroYDev = GyroYDev / 2000;
  GyroZDev = GyroZDev / 2000;

  Serial.print("AccXDev = ");
  Serial.println(AccXDev);
  Serial.print("AccYDev = ");
  Serial.println(AccYDev);
  Serial.print("AccZDev = ");
  Serial.println(AccZDev);
  Serial.print("GyroXDev = ");
  Serial.println(GyroXDev);
  Serial.print("GyroYDev = ");
  Serial.println(GyroYDev);
  Serial.print("GyroZDev = ");
  Serial.println(GyroZDev);
}
