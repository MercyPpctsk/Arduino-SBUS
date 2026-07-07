#include <SBUS.h>

// สร้างออบเจกต์ SBUS ผูกเข้ากับพอร์ต Serial1 ของระบบ (ขา PA9=TX, PA10=RX)
SBUS sbus(Serial1);

unsigned long prevPrint = 0;

void setup() {
  Serial.begin(115200);
  
  // เริ่มต้นใช้งานไลบรารี SBUS
  sbus.begin();
  
  Serial.println("=========================================");
  Serial.println("    SBUS Named Channels Full Example     ");
  Serial.println("=========================================");
}

void loop() {
  // สำคัญที่สุด: ต้องเรียก update() ทุกรอบ loop เพื่อดักเก็บข้อมูลจากรีซีฟเวอร์ตลอดเวลา
  sbus.update();

  // ดึงค่าสัญญาณครบทั้ง 10 แชนเนลแยกเป็นตัวแปรอิสระ
  uint16_t ch1_aileron  = sbus.getChannel(1);  // สติ๊กขวา (ซ้าย-ขวา)
  uint16_t ch2_elevator = sbus.getChannel(2);  // สติ๊กขวา (ขึ้น-ลง)
  uint16_t ch3_throttle = sbus.getChannel(3);  // สติ๊กซ้าย (ขึ้น-ลง) -> คันเร่งเรือ
  uint16_t ch4_rudder   = sbus.getChannel(4);  // สติ๊กซ้าย (ซ้าย-ขวา) -> หางเสือเรือ
  
  uint16_t ch5_swA      = sbus.getChannel(5);  // สวิตช์ SwA
  uint16_t ch6_swB      = sbus.getChannel(6);  // สวิตช์ SwB
  uint16_t ch7_swC      = sbus.getChannel(7);  // สวิตช์ SwC
  uint16_t ch8_swD      = sbus.getChannel(8);  // สวิตช์ SwD
  
  uint16_t ch9_aux1     = sbus.getChannel(9);  // ช่องเสริม Aux 1 (เช่น ลูกบิด VrA/VrB)
  uint16_t ch10_aux2    = sbus.getChannel(10); // ช่องเสริม Aux 2

  // -----------------------------------------------------------------
  // [พื้นที่เขียนโค้ดควบคุมเรือ] 
  // คุณสามารถเอาตัวแปร ch1 - ch10 ไปเขียนเงื่อนไขควบคุมตรงนี้ได้ทันที เช่น:
  // if (ch5_swA > 1800) { ทำงานตอนสับสวิตช์ SwA ลงล่าง }
  // -----------------------------------------------------------------

  unsigned long now = millis();
  // ปริ้นตรวจสอบค่าสัญญาณออกจอ Serial Monitor ทุกๆ 200ms
  if (now - prevPrint >= 200) {
    prevPrint = now;
    
    // ตรวจสอบสัญญาณหลุดก่อนพิมพ์ค่า
    if (sbus.isFailsafe()) {
      Serial.println("\n[RC STATUS] !!! FAILSAFE ACTIVE !!! (Signal Lost)");
      Serial.println("-----------------------------------------");
    } 
    else {
      Serial.println("\n=== [RC NAMED CHANNELS MONITOR] ===");
      
      if (sbus.isFrameLost()) {
        Serial.println("[WARN] Frame Lost Detected!");
      }

      char buf[100];
      
      // 1. แสดงผลกลุ่มสติ๊กบังคับหลัก
      Serial.println("[Sticks]");
      sprintf(buf, "  Aileron  (CH1): %04dus  |  Elevator (CH2): %04dus", ch1_aileron, ch2_elevator);   Serial.println(buf);
      sprintf(buf, "  Throttle (CH3): %04dus  |  Rudder   (CH4): %04dus", ch3_throttle, ch4_rudder);       Serial.println(buf);
      
      // 2. แสดงผลกลุ่มสวิตช์ 2 จังหวะ / 3 จังหวะ
      Serial.println("[Switches]");
      sprintf(buf, "  SwA      (CH5): %04dus  |  SwB      (CH6): %04dus", ch5_swA, ch6_swB);               Serial.println(buf);
      sprintf(buf, "  SwC      (CH7): %04dus  |  SwD      (CH8): %04dus", ch7_swC, ch8_swD);               Serial.println(buf);
      
      // 3. แสดงผลกลุ่มช่องเสริม Aux (ลูกบิดหรือหมุน)
      Serial.println("[Auxiliary]");
      sprintf(buf, "  Aux1     (CH9): %04dus  |  Aux2    (CH10): %04dus", ch9_aux1, ch10_aux2);            Serial.println(buf);
      
      Serial.println("-----------------------------------------");
    }
  }
}