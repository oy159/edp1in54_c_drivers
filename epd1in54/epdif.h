/**
 *  @filename   :   epdif.h
 *  @brief      :   Header file of epdif.cpp providing EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sub-license, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EPDIF_H
#define EPDIF_H

#include "ti_msp_dl_config.h"

// Pin definition for ESP8266 (e.g. NodeMCU)
// Connect display CLK signal to GPIO 14 (Node MCU pin D5)
// Connect display DIN signal to GPIO 13 (Node MCU pin D7)
// Connect display 3.3V to NodeMCU 3V3
// Connect display GND to NodeMCU GND
// Connect other pins as follows:
//      Display       GPIO   ESP32 pin
#define BUSY_PIN        GPIO_LEDS_Epd_BUSY_PIN
#define RST_PIN         GPIO_LEDS_Epd_RST_PIN
#define DC_PIN          GPIO_LEDS_Epd_DC_PIN
#define CS_PIN          GPIO_LEDS_Eps_CS_PIN
#define SDI_PIN         GPIO_LEDS_Epd_SDI_PIN
#define SCK_PIN         GPIO_LEDS_Epd_SCK_PIN

#define LOW                    0
#define HIGH                   1

#define     Line7Port           GPIOA

#define digitalWrite(pin, value)    value?(DL_GPIO_setPins(Line7Port, pin)):(DL_GPIO_clearPins(Line7Port, pin))
#define digitalRead(pin)            DL_GPIO_readPins(Line7Port, pin)
#define delay(x)                    delay_cycles(x*32000)
// #define SPI_transfer(data)          DL_SPI_transmitData8(SPI_0_INST, data)

#define EPD_W21_MOSI_0  digitalWrite(SDI_PIN,LOW)
#define EPD_W21_MOSI_1  digitalWrite(SDI_PIN,HIGH) 

#define EPD_W21_CLK_0   digitalWrite(SCK_PIN,LOW)
#define EPD_W21_CLK_1   digitalWrite(SCK_PIN,HIGH)

#define EPD_W21_CS_0    digitalWrite(CS_PIN,LOW)
#define EPD_W21_CS_1    digitalWrite(CS_PIN,HIGH)

#define EPD_W21_DC_0    digitalWrite(DC_PIN,LOW)
#define EPD_W21_DC_1    digitalWrite(DC_PIN,HIGH)
#define EPD_W21_RST_0   digitalWrite(RST_PIN,LOW)
#define EPD_W21_RST_1   digitalWrite(RST_PIN,HIGH)
#define isEPD_W21_BUSY  digitalRead(BUSY_PIN)

void SPI_Delay(unsigned char xrate);

int  IfInit(void);
void DigitalWrite(int pin, int value);
int  DigitalRead(int pin);
void DelayMs(unsigned int delaytime);
void SpiTransfer(unsigned char data);


#endif
