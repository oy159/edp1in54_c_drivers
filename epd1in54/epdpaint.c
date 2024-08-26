#include "epdpaint.h"


unsigned char*   Paint_image;
int              Paint_width;
int              Paint_height;
int              Paint_rotate;


void Paint_Init(unsigned char* image, int width, int height) {
    Paint_rotate = ROTATE_0;
    Paint_image = image;
    /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
    Paint_width = width % 8 ? width + 8 - (width % 8) : width;
    Paint_height = height;
}


/**
 *  @brief: clear the image
 */
void Paint_Clear(int colored) {
    for (int x = 0; x < Paint_width; x++) {
        for (int y = 0; y < Paint_height; y++) {
            Paint_DrawAbsolutePixel(x, y, colored);
        }
    }
}

/**
 *  @brief: this draws a pixel by absolute coordinates.
 *          this function won't be affected by the rotate parameter.
 */
void Paint_DrawAbsolutePixel(int x, int y, int colored) {
    if (x < 0 || x >= Paint_width || y < 0 || y >= Paint_height) {
        return;
    }
    if (IF_INVERT_COLOR) {
        if (colored) {
            Paint_image[(x + y * Paint_width) / 8] |= 0x80 >> (x % 8);
        } else {
            Paint_image[(x + y * Paint_width) / 8] &= ~(0x80 >> (x % 8));
        }
    } else {
        if (colored) {
            Paint_image[(x + y * Paint_width) / 8] &= ~(0x80 >> (x % 8));
        } else {
            Paint_image[(x + y * Paint_width) / 8] |= 0x80 >> (x % 8);
        }
    }
}

/**
 *  @brief: Getters and Setters
 */
unsigned char* Paint_GetImage(void) {
    return Paint_image;
}

int Paint_GetWidth(void) {
    return Paint_width;
}

void Paint_SetWidth(int width) {
    Paint_width = width % 8 ? width + 8 - (width % 8) : width;
}

int Paint_GetHeight(void) {
    return Paint_height;
}

void Paint_SetHeight(int height) {
    Paint_height = height;
}

int Paint_GetRotate(void) {
    return Paint_rotate;
}

void Paint_SetRotate(int rotate){
    Paint_rotate = rotate;
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void Paint_DrawPixel(int x, int y, int colored) {
    int point_temp;
    if (Paint_rotate == ROTATE_0) {
        if(x < 0 || x >= Paint_width || y < 0 || y >= Paint_height) {
            return;
        }
        Paint_DrawAbsolutePixel(x, y, colored);
    } else if (Paint_rotate == ROTATE_90) {
        if(x < 0 || x >= Paint_height || y < 0 || y >= Paint_width) {
          return;
        }
        point_temp = x;
        x = Paint_width - y;
        y = point_temp;
        Paint_DrawAbsolutePixel(x, y, colored);
    } else if (Paint_rotate == ROTATE_180) {
        if(x < 0 || x >= Paint_width || y < 0 || y >= Paint_height) {
          return;
        }
        x = Paint_width - x;
        y = Paint_height - y;
        Paint_DrawAbsolutePixel(x, y, colored);
    } else if (Paint_rotate == ROTATE_270) {
        if(x < 0 || x >= Paint_height || y < 0 || y >= Paint_width) {
          return;
        }
        point_temp = x;
        x = y;
        y = Paint_height - point_temp;
        Paint_DrawAbsolutePixel(x, y, colored);
    }
}

/**
 *  @brief: this draws a charactor on the frame buffer but not refresh
 */
void Paint_DrawCharAt(int x, int y, char ascii_char, sFONT* font, int colored) {
    int i, j;
    unsigned int char_offset = (ascii_char - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
    const unsigned char* ptr = &font->table[char_offset];

    for (j = 0; j < font->Height; j++) {
        for (i = 0; i < font->Width; i++) {
            if ((*ptr) & (0x80 >> (i % 8))) {
                Paint_DrawPixel(x + i, y + j, colored);
            }
            if (i % 8 == 7) {
                ptr++;
            }
        }
        if (font->Width % 8 != 0) {
            ptr++;
        }
    }
}

/**
*  @brief: this displays a string on the frame buffer but not refresh
*/
void Paint_DrawStringAt(int x, int y, const char* text, sFONT* font, int colored) {
    const char* p_text = text;
    unsigned int counter = 0;
    int refcolumn = x;
    
    /* Send the string character by character on EPD */
    while (*p_text != 0) {
        /* Display one character on EPD */
        Paint_DrawCharAt(refcolumn, y, *p_text, font, colored);
        /* Decrement the column position by 16 */
        refcolumn += font->Width;
        /* Point on the next character */
        p_text++;
        counter++;
    }
}

/**
*  @brief: this draws a line on the frame buffer
*/
void Paint_DrawLine(int x0, int y0, int x1, int y1, int colored) {
    /* Bresenham algorithm */
    int dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int sx = x0 < x1 ? 1 : -1;
    int dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while((x0 != x1) && (y0 != y1)) {
        Paint_DrawPixel(x0, y0 , colored);
        if (2 * err >= dy) {     
            err += dy;
            x0 += sx;
        }
        if (2 * err <= dx) {
            err += dx; 
            y0 += sy;
        }
    }
}

/**
*  @brief: this draws a horizontal line on the frame buffer
*/
void Paint_DrawHorizontalLine(int x, int y, int line_width, int colored) {
    int i;
    for (i = x; i < x + line_width; i++) {
        Paint_DrawPixel(i, y, colored);
    }
}

/**
*  @brief: this draws a vertical line on the frame buffer
*/
void Paint_DrawVerticalLine(int x, int y, int line_height, int colored) {
    int i;
    for (i = y; i < y + line_height; i++) {
        Paint_DrawPixel(x, i, colored);
    }
}

/**
*  @brief: this draws a rectangle
*/
void Paint_DrawRectangle(int x0, int y0, int x1, int y1, int colored) {
    int min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;
    
    Paint_DrawHorizontalLine(min_x, min_y, max_x - min_x + 1, colored);
    Paint_DrawHorizontalLine(min_x, max_y, max_x - min_x + 1, colored);
    Paint_DrawVerticalLine(min_x, min_y, max_y - min_y + 1, colored);
    Paint_DrawVerticalLine(max_x, min_y, max_y - min_y + 1, colored);
}

/**
*  @brief: this draws a filled rectangle
*/
void Paint_DrawFilledRectangle(int x0, int y0, int x1, int y1, int colored) {
    int min_x, min_y, max_x, max_y;
    int i;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;
    
    for (i = min_x; i <= max_x; i++) {
      Paint_DrawVerticalLine(i, min_y, max_y - min_y + 1, colored);
    }
}

/**
*  @brief: this draws a circle
*/
void Paint_DrawCircle(int x, int y, int radius, int colored) {
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do {
        Paint_DrawPixel(x - x_pos, y + y_pos, colored);
        Paint_DrawPixel(x + x_pos, y + y_pos, colored);
        Paint_DrawPixel(x + x_pos, y - y_pos, colored);
        Paint_DrawPixel(x - x_pos, y - y_pos, colored);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos) {
              e2 = 0;
            }
        }
        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}

/**
*  @brief: this draws a filled circle
*/
void Paint_DrawFilledCircle(int x, int y, int radius, int colored) {
    /* Bresenham algorithm */
    int x_pos = -radius;
    int y_pos = 0;
    int err = 2 - 2 * radius;
    int e2;

    do {
        Paint_DrawPixel(x - x_pos, y + y_pos, colored);
        Paint_DrawPixel(x + x_pos, y + y_pos, colored);
        Paint_DrawPixel(x + x_pos, y - y_pos, colored);
        Paint_DrawPixel(x - x_pos, y - y_pos, colored);
        Paint_DrawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, colored);
        Paint_DrawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, colored);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if(-x_pos == y_pos && e2 <= x_pos) {
                e2 = 0;
            }
        }
        if(e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while(x_pos <= 0);
}

/* END OF FILE */
