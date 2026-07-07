#ifndef SBUS_H
#define SBUS_H

#include <Arduino.h>

class SBUS {
  public:
    // Constructor: กำหนดพอร์ต Serial ที่จะใช้ (เช่น Serial1)
    SBUS(HardwareSerial& serial);
    
    // เริ่มต้นระบบคอมมูนิเคชันของ SBUS
    void begin();
    
    // คอยดักอ่านและประมวลผลข้อมูล (ต้องเรียกใน loop() เสมอ)
    void update();
    
    // เรียกอ่านค่า PWM (1000-2000 us) ของแต่ละแชนเนลแบบอิสระ (ระบุเลขช่อง 1 - 10)
    uint16_t getChannel(uint8_t channel);
    
    // เช็คสถานะความปลอดภัยสัญญาณหลุด
    bool isFailsafe();
    bool isFrameLost();

  private:
    HardwareSerial& _serial;
    uint8_t _buffer[25];
    int _index;
    
    uint16_t _channelsRaw[16];
    uint16_t _channelsPWM[16];
    bool _failSafe;
    bool _frameLost;
    
    void parseSBUS();
};

#endif