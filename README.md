### 使用说明

按照添加dsp/include方式添加该文件夹路径，最好放在工作区域的工程目录下

需要修改的部分$epdif.h$中的$PIN$宏定义和$Line7Port$宏定义 
__选择引脚请选择同组GPIO__


具体使用示例，放到主函数即可调用，具体显示数据可将数字转字符再显示

``` C
#include "epd1in54.h"
#include "epdpaint.h"
#include "imagedata.h"
#include "imagedataflower.h"

#include <stdio.h>
#include <string.h>


#define COLORED     0
#define UNCOLORED   1

unsigned char image[1024];
char Str_buf[20];
int main(void)
{
    SYSCFG_DL_init();
    sprintf(Str_buf,"%d",567);
    Paint_Init(image, 0, 0);    // width should be the multiple of 8
    My_Clear();
    My_println(0, Str_buf);
    My_println(5, Str_buf);
    My_Flash();
    while(1){

    }
}

void My_Clear()
{
    Epd_Init();
    Epd_ClearFrameMemory(COLORED);
    Epd_DisplayFrameFull();
    Epd_ClearFrameMemory(COLORED);
    Epd_DisplayFrameFull();
    Epd_Init();
    Paint_SetWidth(200);
    Paint_SetHeight(16);
    Paint_SetRotate(ROTATE_0);

}

//line_number 0-11
void My_println(uint8_t line_number, char *str)
{
    Paint_Clear(COLORED);
    Paint_DrawStringAt(0, 0, str, &Font16, UNCOLORED);
    Epd_SetFrameMemory(Paint_GetImage(), 0, line_number*16, Paint_GetWidth(), Paint_GetHeight());
}

void My_Flash()
{
    Epd_DisplayFrameFull();
    Epd_Sleep();

}


```