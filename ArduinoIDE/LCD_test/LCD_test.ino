#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
// watch video for details of 0x3F I2C address
// end of settings for LCD1602 with I2C


void setup() {
  lcd.begin();  
  lcd.backlight();
  lcd.print("Wifi radio");
  lcd.setCursor(0,1);
  lcd.print("Evropa2");  
  delay(2000); 
}


void loop() {
 // Robojax ESP32 LCD-1602-I2C 
  lcd.clear();// clear previous values from screen
  lcd.print("Wifi radio");
  lcd.setCursor(0,1);
  lcd.print("Stanice:");
  lcd.setCursor(12,1);  
  lcd.print("test");
  delay(2000); 
}
