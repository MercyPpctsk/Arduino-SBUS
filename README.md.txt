# SBUS Library for Arduino
ไลบรารีสำหรับถอดรหัสสัญญาณ SBUS จากรีซีฟเวอร์ (ทดสอบกับ Radiolink R9DS) 
โดยแปลงค่า Raw (309-1690) ออกมาเป็นช่วงมาตรฐาน 1000-2000 us ครบทั้ง 10 แชนเนล

## วิธีใช้งาน
```cpp
#include <SBUS.h>
SBUS sbus(Serial1);

void setup() {
  sbus.begin();
}
void loop() {
  sbus.update();
  uint16_t ch3 = sbus.getChannel(3);
}