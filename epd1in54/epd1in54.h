/**
 *  @filename   :   epd1in54.h
 *  @brief      :   Header file for e-paper display library epd1in54.cpp
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     September 5 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
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

#ifndef EPD1IN54_H
#define EPD1IN54_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       200
#define EPD_HEIGHT      200

// EPD1IN54 commands
#define DRIVER_OUTPUT_CONTROL                       0x01
#define BOOSTER_SOFT_START_CONTROL                  0x0C
#define GATE_SCAN_START_POSITION                    0x0F
#define DEEP_SLEEP_MODE                             0x10
#define DATA_ENTRY_MODE_SETTING                     0x11
#define SW_RESET                                    0x12
#define TEMPERATURE_SENSOR_CONTROL                  0x1A
#define MASTER_ACTIVATION                           0x20
#define DISPLAY_UPDATE_CONTROL_1                    0x21
#define DISPLAY_UPDATE_CONTROL_2                    0x22
#define WRITE_RAM                                   0x24
#define WRITE_VCOM_REGISTER                         0x2C
#define WRITE_LUT_REGISTER                          0x32
#define SET_DUMMY_LINE_PERIOD                       0x3A
#define SET_GATE_TIME                               0x3B
#define BORDER_WAVEFORM_CONTROL                     0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
#define SET_RAM_X_ADDRESS_COUNTER                   0x4E
#define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
#define TERMINATE_FRAME_READ_WRITE                  0xFF

extern const unsigned char lut_full_update[];
extern const unsigned char lut_partial_update[];


extern unsigned long epd_width;
extern unsigned long epd_height;
extern unsigned int reset_pin;
extern unsigned int dc_pin;
extern unsigned int cs_pin;
extern unsigned int busy_pin;
extern const unsigned char* epd_lut;

//void Epd_Pin_Init(void);
void Epd_SendCommand(unsigned char command);
void Epd_SendData(unsigned char data);
void Epd_WaitUntilIdle(void);
int Epd_Init(void);
void Epd_Reset(void);
void Epd_Clear(void);
void Epd_Display(const unsigned char* frame_buffer);
void Epd_DisplayPartBaseImage(const unsigned char* frame_buffer);
void Epd_DisplayPart(const unsigned char* frame_buffer);
void Epd_Sleep();
 void Epd_SetFrameMemory(
    const unsigned char* image_buffer,
    int x,
    int y,
    int image_width,
    int image_height
);
void Epd_SetFrameMemory_A(const unsigned char* image_buffer);
void Epd_SetFrameMemory_B(const unsigned char* image_buffer,unsigned char frame,bool invert);
void Epd_SetBox(
        int x,
        int y,
        int image_width,
        int image_height,
        unsigned char frame,
        unsigned char pattern
    
);
void Epd_ClearFrameMemory(unsigned char color);
void Epd_DisplayFrameFull(void);
void Epd_DisplayFrameFast(void);
void Epd_SetMemoryArea(int x_start, int y_start, int x_end, int y_end);
void Epd_SetMemoryPointer(int x, int y);




#endif /* EPD1IN54_H */

/* END OF FILE */
