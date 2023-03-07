#include <Wire.h>
#include <GyverPortal.h>
#include <GyverOLED.h>

#define AP_SSID "WLAN1-001084"
#define AP_PASS "913C2F40F8881B4"

#define rst 12 
#define Garbage_pin 3


GyverPortal portal;
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

bool PowerFlag, TestFlag, RstFlag, COFlag, RIFlag; 
const int16_t I2C_MASTER = 0x42;
const uint8_t i2c_addr_pic = 0x50;
const char buf_searh_bm1485[5]={0x52, 0x04, 0x00, 0x00, 0x12};
//char buf_read_bm1485[504];
uint8_t dc_dc_dataH = 0x10;
uint8_t dc_dc_dataL = 0x7D;
//10.11V, 9.32V, 9.38V, 9.44V, 9.50V, 9.56V, 9.62V, 9.68V, 9.74V, 9.8V, 10.05V, 10.11V, 10.17V, 10.23V, 10.29V, 10.35V, 10.41V, 10.47V, 10.53V, 10.59V, 10.65V, 10.71V, 10.77V, 10.83V
uint8_t dc_dataL[23] = {0x7D, 0xFF, 0xF5, 0xEB, 0xE1, 0xD7, 0xCD, 0xC3, 0xB9, 0xAF, 0x87, 0x7F, 0x69, 0x5F, 0x55, 0x4B, 0x41, 0x37, 0x2D, 0x23, 0x19, 0x0F, 0x05};
uint8_t modes = 0, StatusI2Cflag;
uint8_t sbuf[512];
bool ledPowerState = false;
int cnt = 0;
uint8_t counters;
int valNum;
int valSelect = 0;
byte Garbage = 0;

void build() 
{
  GP.BUILD_BEGIN();
  GP.THEME(GP_DARK);
  //GP.THEME(GP_LIGHT);
  // список имён компонентов на обновление
  GP.UPDATE("led, lb");

  GP.TITLE("Tester ASIC");
  GP.HR();

  GP_MAKE_BOX(GP.LABEL("Select Asic");   GP.SELECT("", "L3+");  GP.LABEL("Power"); GP.LED_RED("led", ledPowerState););
  GP_MAKE_BOX(GP.LABEL("Select voltage");   GP.SELECT("", "10.11V, 9.32V, 9.38V, 9.44V, 9.50V, 9.56V, 9.62V, 9.68V, 9.74V, 9.8V, 10.05V, 10.17V, 10.23V, 10.29V, 10.35V, 10.41V, 10.47V, 10.53V, 10.59V, 10.65V, 10.71V, 10.77V, 10.83V", valSelect); ); 
  
  GP.BOX_BEGIN();  
  GP.BUTTON_MINI("btn1", "Power", "b1", "GREEN");
  GP.BUTTON_MINI("btn2", "Test");
  GP.BUTTON_MINI("btn3", "RST test");
  GP.BUTTON_MINI("btn4", "CO test");
  GP.BUTTON_MINI("btn5", "RI test");

  GP.BOX_END();
  GP.BREAK();
/*
  GP.BOX_BEGIN(); 
  GP_MAKE_BLOCK(
    GP.LABEL("Power");
    GP.LED_RED("led1", ledPowerState);       
  );
  GP.BOX_END();*/
  //GP.BOX_BEGIN(); 
  
 // GP.BUTTON_MINI("", "1", "", GP_GREEN,"10%");
 // GP.BUTTON_MINI("", "7", "", GP_GREEN,"10%");
 // GP.BUTTON_MINI("", "13", "", GP_GREEN,"10%");
 // GP.BUTTON_MINI("", "19", "", GP_GREEN,"10%");

 // GP.BOX_END();  
  
   //GP_MAKE_BLOCK(
    //GP.LABEL("Checks & LED");
   // GP.BREAK();
    GP.BOX_BEGIN();
    GP.LABEL_BLOCK("1");
    GP.LABEL_BLOCK("7");
    GP.LABEL_BLOCK("13");
    GP.LABEL_BLOCK("19");
    GP.LABEL_BLOCK("25");
    GP.LABEL_BLOCK("31");
    GP.LABEL_BLOCK("37");
    GP.LABEL_BLOCK("43");    
    GP.LABEL_BLOCK("49");
    GP.LABEL_BLOCK("55");
    GP.LABEL_BLOCK("61");
    GP.LABEL_BLOCK("67");
    GP.BOX_END();

    GP.BOX_BEGIN();
    GP.LABEL_BLOCK("2");
    GP.LABEL_BLOCK("8");
    GP.LABEL_BLOCK("14");
    GP.LABEL_BLOCK("20");  
    GP.LABEL_BLOCK("26");
    GP.LABEL_BLOCK("32");
    GP.LABEL_BLOCK("38");
    GP.LABEL_BLOCK("44");
    GP.LABEL_BLOCK("50");
    GP.LABEL_BLOCK("56");
    GP.LABEL_BLOCK("62");
    GP.LABEL_BLOCK("68");
    GP.BOX_END();

    GP.BOX_BEGIN();    
    GP.LABEL_BLOCK("3");
    GP.LABEL_BLOCK("9");
    GP.LABEL_BLOCK("15");
    GP.LABEL_BLOCK("21");
    GP.LABEL_BLOCK("27");
    GP.LABEL_BLOCK("33");
    GP.LABEL_BLOCK("39");
    GP.LABEL_BLOCK("45"); 
    GP.LABEL_BLOCK("51");
    GP.LABEL_BLOCK("57");
    GP.LABEL_BLOCK("63");
    GP.LABEL_BLOCK("69");
    GP.BOX_END();
    
    GP.BOX_BEGIN();
    GP.LABEL_BLOCK("4");
    GP.LABEL_BLOCK("10");
    GP.LABEL_BLOCK("16");
    GP.LABEL_BLOCK("22");    
    GP.LABEL_BLOCK("28");
    GP.LABEL_BLOCK("34");
    GP.LABEL_BLOCK("40");
    GP.LABEL_BLOCK("46");
    GP.LABEL_BLOCK("52");
    GP.LABEL_BLOCK("58");
    GP.LABEL_BLOCK("64");
    GP.LABEL_BLOCK("70");
    GP.BOX_END();
    
    GP.BOX_BEGIN();  
    GP.LABEL_BLOCK("5");
    GP.LABEL_BLOCK("11");
    GP.LABEL_BLOCK("17");
    GP.LABEL_BLOCK("23");
    GP.LABEL_BLOCK("29");
    GP.LABEL_BLOCK("35");
    GP.LABEL_BLOCK("41");
    GP.LABEL_BLOCK("47");    
    GP.LABEL_BLOCK("53");
    GP.LABEL_BLOCK("59");
    GP.LABEL_BLOCK("65");
    GP.LABEL_BLOCK("71");
    GP.BOX_END();
    
    GP.BOX_BEGIN();
    GP.LABEL_BLOCK("6");
    GP.LABEL_BLOCK("12");
    GP.LABEL_BLOCK("18");
    GP.LABEL_BLOCK("24");
    GP.LABEL_BLOCK("30");
    GP.LABEL_BLOCK("36");
    GP.LABEL_BLOCK("42");
    GP.LABEL_BLOCK("48");    
    GP.LABEL_BLOCK("54");
    GP.LABEL_BLOCK("60");
    GP.LABEL_BLOCK("66");
    GP.LABEL_BLOCK("72");
    GP.BOX_END();
    
    GP.HR();

    GP.LABEL("chip: "); 
    GP.LABEL("NAN", "lb");        GP.BREAK();

    GP.AREA_LOG(10);
    //GP.LED("");
    //GP.CHECK("");
    //GP.SWITCH("");
  //);
/*
  GP_MAKE_BLOCK_TAB(
    "Block Tab",
    GP.LABEL("Inputs");
    GP_MAKE_BOX(GP.LABEL("Number");   GP.NUMBER("", "", 123);     );
    GP_MAKE_BOX(GP.LABEL("Float");    GP.NUMBER_F("", "", 3.14);  );
    GP_MAKE_BOX(GP.LABEL("Text");     GP.TEXT("", "", "Hello");   );
    GP_MAKE_BOX(GP.LABEL("Password"); GP.PASS("", "", "Pass");    );
    GP.AREA("", 3, "Text area");
  );
*/
/*
  GP_MAKE_BLOCK_THIN(
    GP_MAKE_BOX(GP.LABEL("Date");   GP.DATE("");  );
    GP_MAKE_BOX(GP.LABEL("Time");   GP.TIME("");  );
    GP_MAKE_BOX(GP.LABEL("Color");  GP.COLOR(""); );
  );
*/
/*
  GP_MAKE_BLOCK_THIN_TAB(
    "Thin Tab",
    GP.LABEL("Upload File/Folder");
    GP_MAKE_BOX(
      GP_CENTER,
      GP.FILE_UPLOAD("");
      GP.FOLDER_UPLOAD("");
    );
  );
*/
/*
  GP_MAKE_BOX(GP.LABEL("Select");   GP.SELECT("", "Some,Drop,List");  );
  GP_MAKE_BOX(GP.LABEL("Slider");   GP.SLIDER("");  );
  GP_MAKE_BOX(GP.LABEL("Spinner");  GP.SPINNER(""); );

  GP.BUTTON("", "Button");
  GP.BUTTON_MINI("", "Btn Mini");
*/
  GP.BUILD_END();
}
//****************************************************************************//
void setup() 
{
  Serial.begin(115200);
  Serial.setTimeout(200);
  //testSerial.begin(BAUD_RATE, SWSERIAL_8N1, D7, D8, false, 95, 11);
  Serial.setRxBufferSize(512);
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  // подключаем конструктор и запускаем
  portal.attachBuild(build);
  portal.attach(action);
  portal.start();
  portal.log.start(30);   // передали размер буфера
  Serial.end();
  pinMode(rst,OUTPUT);     
  digitalWrite(rst, LOW); 
}

void action() {
    if (portal.update()) {
    if (portal.update("led")) portal.answer(ledPowerState);
    if (portal.update("lb")) portal.answer(valNum);
    //portal.updateInt("sel", valSelect);
  }
  if(portal.clickInt("sel", valSelect)){
    portal.log.print("valSelect");
    portal.log.println(valSelect);
  }
  if (portal.clickDown("btn1")){   
    PowerFlag ^= 1;
    TestFlag = 0;
    RstFlag = 0;
    COFlag = 0;
    RIFlag = 0;
    if(PowerFlag){Serial.end(); ledPowerState = true; modes = 0; portal.log.println("Power_on"); Wire.begin();}
    else {ledPowerState = false; portal.log.println("Power_off"); write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x07);}
  } 
  if (portal.clickDown("btn2")) {
    TestFlag ^= 1;
    PowerFlag = 0;
    RstFlag = 0;
    COFlag = 0;
    RIFlag = 0;
    if(TestFlag){
        pinMode(Garbage_pin,INPUT_PULLUP);  
        ledPowerState = true; 
        modes = 0; 
        Garbage = 0; 
        portal.log.println("Test_on"); 
        Wire.begin();
        }
    else {Serial.end(); ledPowerState = false; portal.log.println("Test_off"); write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x07); }
  }
  if (portal.clickDown("btn3")) {
    RstFlag ^= 1,
    TestFlag = 0;
    PowerFlag = 0;
    COFlag = 0;
    RIFlag = 0;
    if(RstFlag){Serial.end(); ledPowerState = true; modes = 0; portal.log.println("RST test on"); Wire.begin();}
    else {ledPowerState = false; portal.log.println("RST test off"); write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x07); }  
  }
  if (portal.clickDown("btn4")) {
    COFlag ^= 1,
    RstFlag = 0; 
    TestFlag = 0;
    PowerFlag = 0;
    RIFlag = 0;    
    if(COFlag){ ledPowerState = true; modes = 0; portal.log.println("CO test on"); Wire.begin();}
    else {ledPowerState = false; portal.log.println("CO test off"); write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x07); }      
  }
  if (portal.clickDown("btn5")) {
    RIFlag ^= 1,
    RstFlag = 0;
    TestFlag = 0;
    PowerFlag = 0;
    COFlag = 0;
    if(RIFlag){ ledPowerState = true; modes = 0; portal.log.println("RI test on"); Wire.begin();}
    else {ledPowerState = false; portal.log.println("RI test off"); write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x07); }      
  }
}
//****************************************************************************//

void loop() {
  portal.tick();
  if (TestFlag) Asic_test();
  else if (PowerFlag) Asic_power();
        else if (RstFlag) Asic_rst_test();
              else if (COFlag) Asic_COtest();
                    else if (RIFlag) Asic_RItest();
}
//****************************************************************************//

void Asic_power() 
{
  static uint32_t tmr, period;
  
  if(millis() - tmr >= period)
  {
    tmr = millis();
    switch (modes)
    {
      case 0: write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x07); period = 1000; if(StatusI2Cflag == 0) modes = 1; portal.log.println("pic 0xAA, 0x07"); break;
      case 1: write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x06); period = 1000; if(StatusI2Cflag == 0) modes = 2; portal.log.println("pic 0xAA, 0x06"); break;
      case 2: write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x15); write_Asic_i2c(0x01); period = 1000; if(StatusI2Cflag == 0) modes = 3; portal.log.println("pic 0xAA, 0x15, 0x01"); break;
      case 3: write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x16); period = 6000; if(StatusI2Cflag == 0) modes = 4; portal.log.println("pic 0xAA, 0x16"); break;
      case 4: digitalWrite(rst, HIGH); period = 500; modes = 5; break;
      case 5: digitalWrite(rst, LOW); period = 50; modes = 6; break;
      case 6: digitalWrite(rst, HIGH); period = 500; modes = 7; break;
      case 7: digitalWrite(rst, LOW); Asic_setup_dc_dc(dc_dc_dataH, dc_dataL[valSelect]);  period = 5000; if(StatusI2Cflag == 0) modes = 8; break;
      case 8: write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x16); period = 9800; if(StatusI2Cflag != 0) {modes = 0; portal.log.println("no pic respond");} break;
    }
    if(StatusI2Cflag != 0) portal.log.println("no pic respond");
  }
}

void Asic_init() 
{
  static uint32_t tmr, period;
  
  if(millis() - tmr >= period)
  {
    tmr = millis();
    switch (modes)
    {
      case 10: write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x16); period = 9800; break;
      case 0: write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x07); period = 1000;  if(StatusI2Cflag == 0) modes = 1; portal.log.println("pic 0xAA, 0x07"); break;
      case 1: write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x06); period = 1000; if(StatusI2Cflag == 0) modes = 2; portal.log.println("pic 0xAA, 0x06"); break;
      case 2: write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x15); write_Asic_i2c(0x01); period = 1000; if(StatusI2Cflag == 0) modes = 3; portal.log.println("pic 0xAA, 0x15, 0x01"); break;
      case 3: write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x16); period = 6000; if(StatusI2Cflag == 0) modes = 4; portal.log.println("pic 0xAA, 0x16"); break;
      case 4: digitalWrite(rst, HIGH); period = 500; modes = 5; portal.log.println("Reset: LOW"); break;
      case 5: digitalWrite(rst, LOW); period = 50; modes = 6; portal.log.println("Reset: HIGH"); break;
      case 6: digitalWrite(rst, HIGH); period = 500; modes = 7; portal.log.println("Reset: LOW"); break;
      case 7: digitalWrite(rst, LOW); Asic_setup_dc_dc(dc_dc_dataH, dc_dataL[valSelect]); period = 3000; if(StatusI2Cflag == 0) {modes = 8;} portal.log.println("Reset: HIGH"); for (int i = 0; i < 512; i++) {sbuf[i] = 0;} portal.log.println("pic set voltage"); break;
      case 8: Serial.begin(115200); Serial.flush(); Serial.write(buf_searh_bm1485, 5); period = 3000; modes = 9; portal.log.println("searh_bm1485"); break;
      case 9: write_Asic_i2c(0x55); write_Asic_i2c(0xAA); write_Asic_i2c(0x16); period = 9800; if(StatusI2Cflag == 0) {modes = 10; portal.log.println("pic 0xAA, 0x16");} break;    
    }

    if(StatusI2Cflag != 0) portal.log.println("no pic respond");
  }
  static uint8_t prevGarbage, Garbages;  
  if(modes == 7){ 
    Garbages = digitalRead(Garbage_pin);
    if(Garbages != prevGarbage) prevGarbage = Garbages; Garbage++;
  }
  else prevGarbage = Garbages; 
} 

void Asic_test() {

  static uint32_t tmr;
  uint8_t mod = 0, count;

  Asic_init();

  if((millis() - tmr >= 3000) & (modes == 10))
  {
    tmr = millis();
    if (Serial.available()) {     // если данные получены
      size_t len = Serial.available();
      Serial.readBytes(sbuf, len);
      cnt = len;
    }
    
    portal.log.print(counters++);
    portal.log.print("->chip:");
    portal.log.println(cnt/7);
    
    mod = 0;
    
    for (int i = 0; i < 512; i++) {

      if((sbuf[i] == 0x14) & (mod == 0)) mod = 1;
      else if(mod == 1) { if(sbuf[i] == 0x85) mod = 2;
                          else mod = 0;}
           else if(mod == 2) { if((sbuf[i] == 0x9C) | (sbuf[i] == 0x94) | (sbuf[i] == 0x98)) mod = 3;
                               else mod = 0;} 
                else if((mod == 3)|(mod == 4)| (mod == 5)) { if(sbuf[i] == 00) mod++;
                                                             else mod = 0;}                                     
                     else if(mod == 6) { if((sbuf[i] == 0x18) | (sbuf[i] == 0x08) | (sbuf[i] == 0x10)) mod++;
                                         else mod = 0;}
      if(mod > 6){count++; mod = 0;}     
      sbuf[i] = 0;
    }
    valNum = count;
    portal.log.print("bm1485:"); portal.log.println(count);
    count = 0;
    //portal.log.print("Garbage:"); portal.log.println(Garbage);
    //Garbage = 0;   
    Serial.write(buf_searh_bm1485, 5);
  }
}

void Asic_COtest() {
  static uint32_t tmr;

  Asic_init();

  if((millis() - tmr >= 1000) & (modes == 10))
  {
    tmr = millis();
    //cnt = buf.available()*2;
    //portal.log.print("chip:"); portal.log.println(cnt);
    //while (sbuf.available()) {
      //portal.log.print(sbuf.read());
      //portal.log.println(";");
    //}
    //portal.log.println("----------------");
    //sbuf.clear();   
    Serial.write(buf_searh_bm1485, 5);
  }
}

void Asic_RItest() {
  static uint32_t tmr;

  Asic_init();

  if((millis() - tmr >= 5000) & (modes == 10))
  {
    tmr = millis();
    //cnt = sbuf.available()*2/7;
    //portal.log.print("chip:"); portal.log.println(cnt);
    //while (sbuf.available()) {
     // portal.log.print(sbuf.read());
     // portal.log.print(";");
    //}
    //portal.log.println("");
    //portal.log.println("----------------");
    //sbuf.clear();   
    Serial.write(buf_searh_bm1485, 5);
  }
}

void Asic_rst_test()
{
  static uint32_t tmr, period;
  static bool mode_rst = 0;
  
  Asic_init();
  
  if((millis() - tmr >= period) & (modes == 10))
  {
    tmr = millis();
    if(!mode_rst) 
    {
      digitalWrite(rst, HIGH); 
      period = 50; 
      mode_rst = true;
    }
    else 
    { 
      digitalWrite(rst, LOW);
      period = 50; 
      modes = false;
    }
  }
}

void write_Asic_i2c(uint8_t data) 
{  
  Wire.beginTransmission(i2c_addr_pic);
  Wire.write(data);   
  StatusI2Cflag = Wire.endTransmission();
}

void Asic_setup_dc_dc(int dataH, int dataL) 
{
  write_Asic_i2c(0x55);
  write_Asic_i2c(0xAA);
  write_Asic_i2c(dataH);
  write_Asic_i2c(dataL);
}
