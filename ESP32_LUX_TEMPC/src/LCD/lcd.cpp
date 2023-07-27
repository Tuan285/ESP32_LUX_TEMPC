#define DATA
#include <init.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
extern Value_t data;
extern bool modeAuto;
extern bool LCDclear;

char gas[5];
char lux[5];
void initLCD()
{
    lcd.init();
    lcd.backlight();
}
void showLCD()
{
    if (LCDclear == true)
    {
        lcd.clear();
        LCDclear = false;
    }
    lcd.setCursor(0, 0);
    lcd.print("ND: ");
    lcd.print(data.tempC);

    lcd.setCursor(0, 1);
    lcd.print("DA: ");
    lcd.print(data.Humidity);

    sprintf(lux, " %d ", data.Lux);
    lcd.setCursor(0, 2);
    lcd.print("LUX:");
    lcd.print(lux);

    sprintf(gas, " %d ", data.Gas);
    lcd.setCursor(0, 3);
    lcd.print("GAS:");
    lcd.print(gas);
    if (modeAuto == 1)
    {
        LCDAuto();
    }
}
void LCDAuto()
{
    lcd.setCursor(12, 0);
    lcd.print(data.minTempC);

    lcd.setCursor(16, 0);
    lcd.print(data.maxTempC);

    lcd.setCursor(12, 1);
    lcd.print(data.minHumidity);

    lcd.setCursor(16, 1);
    lcd.print(data.maxHumidity);

    lcd.setCursor(12, 2);
    lcd.print(data.minLux);

    lcd.setCursor(16, 2);
    lcd.print(data.maxLux);

    lcd.setCursor(12, 3);
    lcd.print(data.minGas);

    lcd.setCursor(16, 3);
    lcd.print(data.maxGas);
}