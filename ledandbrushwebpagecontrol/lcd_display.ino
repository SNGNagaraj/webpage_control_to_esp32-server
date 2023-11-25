void display()
{ 
   lcd.setCursor(0,0);
   lcd.clear();
   lcd.print("HILD ENERGY");
   delay(1000);
}

void lcdwifi()
{
  lcd.setCursor(0,0);
  lcd.clear();
 // lcd.setCursor(0,0);
  lcd.print("IP:");
  lcd.setCursor(3,0);
  lcd.print(WiFi.localIP());
 // lcd.setCursor(0,0);
 // lcd.print("IP:");
  //lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("WIFI RSSI:");
  lcd.setCursor(11,1);
 // lcd.clear();
  lcd.println(WiFi.RSSI());
  lcd.setCursor(14,1);
  lcd.print("dB");
} 

void lcdmotordrivestatus()
{
                //delay(1000);
                lcd.setCursor(0,0);
                lcd.clear();
                lcd.print("MotorDriveStatus");
                //client.println(MotorDrive);
                lcd.setCursor(0,1);
                //lcd.clear();
                lcd.print(output26State);
                delay(3000);
}
void lcdbrushdrivestatus()
{
                lcd.setCursor(0,0);
                lcd.clear();
                lcd.print("BrushDriveStatus");
                //client.println(MotorDrive);
                lcd.setCursor(0,1);
                //lcd.clear();
                lcd.print(output27State);
                delay(3000);

}







