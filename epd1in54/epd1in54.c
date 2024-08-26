/**
 *  @filename   :   epd1in54.cpp
 *  @brief      :   Implements for e-paper library
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

#include <stdlib.h>
#include "epd1in54.h"
#define reset_pin    RST_PIN
#define dc_pin       DC_PIN
#define cs_pin       CS_PIN
#define busy_pin     BUSY_PIN

unsigned long epd_width = EPD_WIDTH;
unsigned long epd_height = EPD_HEIGHT;


void Epd_SendCommand(unsigned char command)
{
    DigitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd_SendData(unsigned char data)
{
    DigitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd_WaitUntilIdle(void)
{
    while(DigitalRead(busy_pin) == 1) {      //LOW: idle, HIGH: busy
        DelayMs(5);
         //Serial.print("e-Paper WaitUntilIdle");
    }
    //DelayMs(200);
}

int Epd_Init(void)
{
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }
    /* EPD hardware init start */
    Epd_Reset();

    Epd_WaitUntilIdle();
    Epd_SendCommand(0x12);  //SWRESET
    Epd_WaitUntilIdle();

    Epd_SendCommand(0x01); //Driver output control
    Epd_SendData(0xC7);
    Epd_SendData(0x00);
    //SendData(0x01);
    Epd_SendData(0x0); //change direction was 1 
      
    Epd_SendCommand(0x11); //data entry mode
    Epd_SendData(0x01);

    Epd_SendCommand(0x44); //set Ram-X address start/end position
    Epd_SendData(0x00);
    Epd_SendData(0x18);    //0x0C-->(18+1)*8=200

    Epd_SendCommand(0x45); //set Ram-Y address start/end position
    Epd_SendData(0xC7);   //0xC7-->(199+1)=200
    Epd_SendData(0x00);
    Epd_SendData(0x00);
    Epd_SendData(0x00);

    Epd_SendCommand(0x3C); //BorderWavefrom
    Epd_SendData(0x01);

    Epd_SendCommand(0x18);
    Epd_SendData(0x80);

    Epd_SendCommand(0x22); // //Load Temperature and waveform setting.
    Epd_SendData(0XB1);
    Epd_SendCommand(0x20);

    Epd_SendCommand(0x4E);   // set RAM x address count to 0;
    Epd_SendData(0x00);
    Epd_SendCommand(0x4F);   // set RAM y address count to 0X199;
    Epd_SendData(0xC7);
    Epd_SendData(0x00);
    Epd_WaitUntilIdle();
    /* EPD hardware init end */

    return 0;
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd_Sleep();
 */
void Epd_Reset(void)
{
    DigitalWrite(reset_pin, LOW);                //module reset    
    DelayMs(5);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(5);

    return;
}

void Epd_Clear(void)
{
    int w, h;
    w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    h = EPD_HEIGHT;
    Epd_SendCommand(0x24);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            Epd_SendData(0xff);
        }
    }

    //DISPLAY REFRESH
    Epd_SendCommand(0x22);
    Epd_SendData(0xF7);
    Epd_SendCommand(0x20);
    Epd_WaitUntilIdle();
}

void Epd_Display(const unsigned char* frame_buffer)
{
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    if (frame_buffer != NULL) {
        Epd_SendCommand(0x24);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                Epd_SendData(pgm_read_byte(&frame_buffer[i + j * w]));
            }
        }
    }

    //DISPLAY REFRESH
    Epd_SendCommand(0x22);
    Epd_SendData(0xF7);
    Epd_SendCommand(0x20);
    Epd_WaitUntilIdle();
}

void Epd_DisplayPartBaseImage(const unsigned char* frame_buffer)
{
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    if (frame_buffer != NULL) {
        Epd_SendCommand(0x24);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                Epd_SendData(frame_buffer[i + j * w]);
            }
        }

        Epd_SendCommand(0x26);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                Epd_SendData(frame_buffer[i + j * w]);
            }
        }
    }

    //DISPLAY REFRESH
    Epd_SendCommand(0x22);
    Epd_SendData(0xFF);
    Epd_SendCommand(0x20);
    Epd_WaitUntilIdle();
}

void Epd_DisplayPart(const unsigned char* frame_buffer)
{
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    if (frame_buffer != NULL) {
        Epd_SendCommand(0x24);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                Epd_SendData(frame_buffer[i + j * w]);
            }
        }
    }

    //DISPLAY REFRESH
    Epd_SendCommand(0x22);
    Epd_SendData(0xFF);
    Epd_SendCommand(0x20);
    Epd_WaitUntilIdle();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the
 *          deep-sleep mode to save power.
 *          The deep sleep mode would return to standby by hardware reset.
 *          The only one parameter is a check code, the command would be
 *          executed if check code = 0xA5.
 *          You can use Epd_Init() to awaken
 */
void Epd_Sleep()
{
    Epd_SendCommand(0x10); //enter deep sleep
    Epd_SendData(0x01);
    DelayMs(200);

    DigitalWrite(reset_pin, LOW);
}

/* Adding missing functions from V1   */
 void Epd_SetFrameMemory(
    const unsigned char* image_buffer,
    int x,
    int y,
    int image_width,
    int image_height
) {
    int x_end;
    int y_end;
    unsigned char byte1;
    
    if (
       
        x < 0 || image_width < 0 ||
        y < 0 || image_height < 0
    ) {
        return;
    }
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= epd_width) {
        x_end = epd_width - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= epd_height) {
        y_end = epd_height - 1;
    } else {
        y_end = y + image_height - 1;
    }
    Epd_SetMemoryArea(x, y, x_end, y_end);
    /* set the frame memory line by line */
    for (int j = y; j <= y_end; j++) {
        
         Epd_SetMemoryPointer(x, j);
        for (uint8_t command = 0x24; true; command = 0x26)
        {
           Epd_SendCommand(command);
           for (int i = 0 ; i < image_width / 8; i++) {
             byte1 =  image_buffer[i + (j-y) * (image_width / 8)];
            if(command == 0x26)
              Epd_SendData(~byte1);
            else  
              Epd_SendData((byte1));
        }
        if(command == 0x26) break;
      }
    }
} 

void Epd_SetFrameMemory_A(const unsigned char* image_buffer) {
  unsigned char byte1;
  
    Epd_SetMemoryArea(0, 0, epd_width - 1, epd_height - 1);
 
    /* set the frame memory line by line */
    for (int j = 0; j <= epd_height - 1; j++) {
        
        Epd_SetMemoryPointer(0, j);
        for (uint8_t command = 0x24; true; command = 0x26)
        {
           Epd_SendCommand(command);
            
           for (int i = 0 ; i < epd_width / 8; i++) {
            byte1 =  image_buffer[i + j * (epd_width / 8)];
            if(command == 0x26)
              Epd_SendData(~byte1);
            else  
              Epd_SendData((byte1));
              
         
        }
        if(command == 0x26) break;
      }
    }
}
void Epd_SetFrameMemory_B(const unsigned char* image_buffer,unsigned char frame,bool invert) {
  unsigned char byte1;
  
    Epd_SetMemoryArea(0, 0, epd_width - 1, epd_height - 1);
 
    /* set the frame memory line by line */
    for (int j = 0; j <= epd_height - 1; j++) {
        
        Epd_SetMemoryPointer(0, j);
        
           Epd_SendCommand(frame);
            
           for (int i = 0 ; i < epd_width / 8; i++) {
            byte1 =  image_buffer[i + j * (epd_width / 8)];
            if(invert)
              Epd_SendData(~byte1);
            else  
              Epd_SendData((byte1));
           }
    }
}

void Epd_SetBox(
        int x,
        int y,
        int image_width,
        int image_height,
        unsigned char frame,
        unsigned char pattern
    
    )
    {
  int x_end;
    int y_end;
    
    if (
       
        x < 0 || image_width < 0 ||
        y < 0 || image_height < 0
    ) {
        return;
    }
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= epd_width) {
        x_end = epd_width - 1;
    } else {
        x_end = x + image_width - 1;
    }
    if (y + image_height >= epd_height) {
        y_end = epd_height - 1;
    } else {
        y_end = y + image_height - 1;
    }
       
    Epd_SetMemoryArea(x, y, x_end, y_end);
  
         

    
      for (int j = y; j < y_end; j++) {
          Epd_SetMemoryPointer(x, j);
          Epd_SendCommand(frame);
          for (int i = 0; i < image_width / 8; i++) {
            Epd_SendData(pattern);
         }
      }
  
      
    }
    
void Epd_ClearFrameMemory(unsigned char color) {
   Epd_SetMemoryArea(0, 0, epd_width - 1, epd_height - 1);

    for (int j = 0; j < epd_height; j++) {
        Epd_SetMemoryPointer(0, j);
         Epd_SendCommand(0x24);
        for (int i = 0; i < epd_width / 8; i++) {
           Epd_SendData(color);
        }
    }
    
    for (int j = 0; j < epd_height; j++) {
         Epd_SetMemoryPointer(0, j);
         Epd_SendCommand(0x26);
        for (int i = 0; i < epd_width / 8; i++) {
            Epd_SendData(~color);
        }
    }
}

void Epd_DisplayFrameFull(void) {
    //DISPLAY REFRESH
    Epd_SendCommand(0x22);
    Epd_SendData(0xF7);
    Epd_SendCommand(0x20);
    Epd_WaitUntilIdle();
}


void Epd_DisplayFrameFast(void) {
    //DISPLAY REFRESH
    Epd_SendCommand(0x22);
    Epd_SendData(0xFF);
    Epd_SendCommand(0x20);
    Epd_WaitUntilIdle();
}

void Epd_SetMemoryArea(int x_start, int y_start, int x_end, int y_end) {
    Epd_SendCommand(0x44);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    Epd_SendData((x_start >> 3) & 0xFF);
    Epd_SendData((x_end >> 3) & 0xFF);
    Epd_SendCommand(0x45);
    Epd_SendData(y_start & 0xFF);
    Epd_SendData((y_start >> 8) & 0xFF);
    Epd_SendData(y_end & 0xFF);
    Epd_SendData((y_end >> 8) & 0xFF);
}

void Epd_SetMemoryPointer(int x, int y) {
    Epd_SendCommand(0x4E);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    Epd_SendData((x >> 3) & 0xFF);
    Epd_SendCommand(0x4F);
    Epd_SendData(y & 0xFF);
    Epd_SendData((y >> 8) & 0xFF);
  //  WaitUntilIdle();
}



