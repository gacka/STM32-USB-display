
#ifndef __LCDST7565r_H 
#define __LCDST7565r_H

//#include "iolpc1114.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "delay.h"
#include "ascii_unicode.h"
#include <string.h>
#include <stdio.h>

//#define LCD_BLUE
#define LCD_WHITE

#ifdef LCD_WHITE
  #define LCD_notinverted 0xA6
  #define LCD_inverted    0xA7
  #define LCD_contrast    20
#endif


#ifdef LCD_BLUE
  #define LCD_notinverted 0xA7
  #define LCD_inverted    0xA6
  #define LCD_contrast    22
#endif

#define LCD_CTRL_PORT	GPIOA
#define LCD_SI         GPIO_PIN_5
#define LCD_SI_DATA    GPIOA->ODR

#define LCD_SCL        GPIO_PIN_4
#define LCD_SCL_DATA   GPIOA->ODR

#define LCD_A0         GPIO_PIN_3
#define LCD_A0_DATA    GPIOA->ODR

#define LCD_CS         GPIO_PIN_0
#define LCD_CS_DATA    GPIOA->ODR

#define LCD_RST       GPIO_PIN_2
#define LCD_RST_DATA   GPIOA->ODR

#define LCD_LIGHT         GPIO_PIN_1
#define LCD_PORT		      GPIOA
#define LCD_LIGHT_DATA    GPIOA->ODR

extern unsigned char lcd_displaybuffer[1024];
//extern char buffer[80];

void lcd_init(void);
void lcd_io_init(void);
void lcd_turnOff(void);

void lcd_sendspibyte(unsigned char v);
void lcd_updatewholedisplay(uint8_t);
void clear_lcd (void);

void rectangle (unsigned char X,unsigned char Y, unsigned char H, unsigned char W, unsigned char Mode);
void displayText_unicode(unsigned char X, unsigned char Y,const char *c,const struct FONT_TYPE *font,unsigned char mode);
void displayText_GLCD_v_unicode(unsigned char X, unsigned char Y,char *c,const struct FONT_TYPE *font);
void displayTextScroll_unicode(unsigned char Xstart, unsigned char Ystart,unsigned int pix_offset,unsigned char window,const char *c,const struct FONT_TYPE *font,unsigned char mode);

uint16_t getStringLength_pixels_unicode(char *c_in, const struct FONT_TYPE *font_ptr);
uint8_t cutTextToWidth_unicode(char *c_in, char *c_out, uint8_t width, const struct FONT_TYPE *font_ptr, uint8_t cutToLastSpaceFlag);
uint8_t divdeStringToNLines_unicode(char *c_in, char **lines, uint8_t width,const struct FONT_TYPE *font_ptr, uint8_t line_no, uint8_t cutToLastSpaceFlag);
uint8_t getXpositionCenteredText_unicode(uint8_t startX, uint8_t stopX, char *c_ptr, const struct FONT_TYPE *font_ptr); // wyliczenie wspolrzednej X wysrodkowanego napisu
uint8_t getDivdeCharIndexes(const char *c_in, unsigned int *resultIndexArray_ptr, unsigned char *resultLenArray_ptr, unsigned char *resultLinesNbr, unsigned char line_width, const struct FONT_TYPE *font_ptr);
void displayTextScrollVertical_unicode(unsigned char Xstart, unsigned char Ystart, unsigned int pix_offset, unsigned char windowWidth, unsigned char windowHeight, const char *c,const struct FONT_TYPE *font,unsigned char mode); // to center line set 2 bit in mode byte
void displayText_GLCD(unsigned char X, unsigned char Y,char *c,struct FONT_TYPE font,uint8_t isBlack, char delimiter_char);
//void displayText_GLCD(unsigned char X, unsigned char Y,char *c,struct FONT_TYPE font,uint8_t isBlack, char delimiter_char);
//void displayText_GLCD_I(unsigned char X, unsigned char Y,char *c,uint8_t FONT_INDEX);

//uint8_t divdeStringToNLines_withRest(char *c_in, char **lines, char *restBuffer, uint8_t width,const struct FONT_TYPE *font_ptr, uint8_t line_no);
//uint8_t divdeStringToNLines2(char *c_in, char **lines, uint8_t width,const struct FONT_TYPE *font_ptr, uint8_t line_no);

//uint8_t cutTextToWidth(char *c_in, char *c_out, uint8_t width, const struct FONT_TYPE *font_ptr);

//uint8_t divdeStringToThreeLines(char *c_in, char *lines[3], uint8_t width,const struct FONT_TYPE *font_ptr); 
//uint8_t divdeStringToNLines(char *c_in, char **lines, uint8_t width,const struct FONT_TYPE *font_ptr, uint8_t line_no, uint8_t cutToLastSpaceFlag);
//uint8_t getTextWidth(char *c_ptr, const struct FONT_TYPE *font_ptr);



#endif /* end __LCDST7565r_H */
