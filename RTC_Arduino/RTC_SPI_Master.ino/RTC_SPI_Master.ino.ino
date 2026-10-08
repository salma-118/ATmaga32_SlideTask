#include <Wire.h>
#include <SPI.h>

#define RTC_ADDRESS 0x68
#define SS_PIN 10

byte bcdToDecimal(byte value)
{
    return ((value >> 4) * 10) + (value & 0x0F);
}

byte readRTCRegister(byte reg)
{
    Wire.beginTransmission(RTC_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(RTC_ADDRESS, 1);

    if (Wire.available())
        return Wire.read();

    return 0;
}

void setup()
{
    // I2C
    Wire.begin();

    // SPI
    SPI.begin();

    // ATmega328P PB2 / Arduino D10 → ATmega32 SS
    pinMode(SS_PIN, OUTPUT);
    digitalWrite(SS_PIN, HIGH);

    SPI.beginTransaction(
        SPISettings(1000000, MSBFIRST, SPI_MODE0)
    );
}

void loop()
{
    // DS1307 registers
    byte second = bcdToDecimal(readRTCRegister(0x00) & 0x7F);
    byte minute = bcdToDecimal(readRTCRegister(0x01) & 0x7F);

    byte hour = bcdToDecimal(
        readRTCRegister(0x02) & 0x3F
    );

    byte day = bcdToDecimal(
        readRTCRegister(0x04) & 0x3F
    );

    byte month = bcdToDecimal(
        readRTCRegister(0x05) & 0x1F
    );

    byte year = bcdToDecimal(
        readRTCRegister(0x06)
    );

    // -----------------------------------------
    // SPI FRAME
    // [AA][DAY][MONTH][YEAR][HOUR][MIN][SEC]
    // -----------------------------------------

    digitalWrite(SS_PIN, LOW);

    SPI.transfer(0xAA);

    SPI.transfer(day);
    SPI.transfer(month);
    SPI.transfer(year);

    SPI.transfer(hour);
    SPI.transfer(minute);
    SPI.transfer(second);

    digitalWrite(SS_PIN, HIGH);

    delay(1000);
}