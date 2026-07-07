#include "SBUS.h"

SBUS::SBUS(HardwareSerial& serial) : _serial(serial), _index(0), _failSafe(false), _frameLost(false) {
  memset(_channelsRaw, 0, sizeof(_channelsRaw));
  // ตั้งค่า Default ตอนยังไม่มีสัญญาณให้สติ๊กอยู่ตรงกลาง (1500) ส่วนคันเร่งช่อง 3 ให้เป็นต่ำสุด (1000)
  for (int i = 0; i < 16; i++) {
    _channelsPWM[i] = (i == 2) ? 1000 : 1500;
  }
}

void SBUS::begin() {
  _serial.begin(100000, SERIAL_8E2);
}

void SBUS::update() {
  while (_serial.available() > 0) {
    uint8_t b = _serial.read();
    
    if (_index == 0 && b != 0x0F) {
      continue; 
    }
    
    _buffer[_index++] = b;
    
    if (_index == 25) {
      uint8_t footer = _buffer[24];
      if (footer == 0x00 || footer == 0x04 || footer == 0x14 || footer == 0x24 || footer == 0x34) {
        parseSBUS();
      }
      _index = 0;
    }
  }
}

void SBUS::parseSBUS() {
  _channelsRaw[0]  = ((_buffer[1]       | _buffer[2]  << 8) & 0x07FF);
  _channelsRaw[1]  = ((_buffer[2] >> 3  | _buffer[3]  << 5) & 0x07FF);
  _channelsRaw[2]  = ((_buffer[3] >> 6  | _buffer[4]  << 2 | _buffer[5] << 10) & 0x07FF);
  _channelsRaw[3]  = ((_buffer[5] >> 1  | _buffer[6]  << 7) & 0x07FF);
  _channelsRaw[4]  = ((_buffer[6] >> 4  | _buffer[7]  << 4) & 0x07FF);
  _channelsRaw[5]  = ((_buffer[7] >> 7  | _buffer[8]  << 1 | _buffer[9] << 9) & 0x07FF);
  _channelsRaw[6]  = ((_buffer[9] >> 2  | _buffer[10] << 6) & 0x07FF);
  _channelsRaw[7]  = ((_buffer[10] >> 5 | _buffer[11] << 3) & 0x07FF);
  _channelsRaw[8]  = ((_buffer[12]      | _buffer[13] << 8) & 0x07FF);
  _channelsRaw[9]  = ((_buffer[13] >> 3 | _buffer[14] << 5) & 0x07FF);

  _frameLost = _buffer[23] & 0x04;
  _failSafe  = _buffer[23] & 0x08;

  // แปลงช่วงสัญญาณ 309-1690 ของรีโมทจริงให้เป็น 1000-2000 us พอดีเป๊ะ
  for (int i = 0; i < 10; i++) {
    _channelsPWM[i] = map(_channelsRaw[i], 309, 1690, 1000, 2000);
    if (_channelsPWM[i] < 1000) _channelsPWM[i] = 1000;
    if (_channelsPWM[i] > 2000) _channelsPWM[i] = 2000;
  }
}

uint16_t SBUS::getChannel(uint8_t channel) {
  // บังคับให้อ่านได้เฉพาะช่อง 1 ถึง 10 (ดัดกลับเป็น Index 0-9 ของอาร์เรย์)
  if (channel < 1 || channel > 10) return 1500; 
  return _channelsPWM[channel - 1];
}

bool SBUS::isFailsafe()  { return _failSafe; }
bool SBUS::isFrameLost() { return _frameLost; }