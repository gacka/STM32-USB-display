#include "lcdst7565r_unicode.h"



unsigned char  lcd_displaybuffer[1024];
//unsigned int i;
//char buffer[80];

void clear_lcd (void)
{
	uint16_t i;
   for (i=0; i<1024; i++) lcd_displaybuffer[i]=0;
}
//------------------------------------------------------------------------
void rectangle (unsigned char X,unsigned char Y, unsigned char height, unsigned char width, unsigned char Mode){

	unsigned char page;
	signed char y;
	unsigned char x;

	for (y=Y; y<Y+height; y++){	
		page= y/8;
		for (x=X; x<X+width; x++){
			if (Mode==1) 	
				lcd_displaybuffer[page * 128 + x] |=  (1 << ( y % 8));	  // display bar
			else if(Mode==0)
					lcd_displaybuffer[page * 128 + x] &= ~(1 << ( y % 8));	  //clr bar
			else{
					if( (y<Y +1) ||(y==Y+height-1) ) lcd_displaybuffer[page * 128 + x] |=  (1 << ( y % 8));
					else if( (x> X+width-2) || (x<X+1) ) lcd_displaybuffer[page * 128 + x] |=  (1 << ( y % 8));
				}
		}
	}
}
//------------------------------------------------------------------------


void displayTextScrollVertical_unicode(unsigned char Xstart, unsigned char Ystart, unsigned int pix_offset, unsigned char windowWidth, unsigned char windowHeight, const char *c,const struct FONT_TYPE *font,unsigned char mode)
{
	unsigned int resultIndexArray[10];
	unsigned char resultLenArray[10];
	unsigned char resLinesNbr = 0;
	unsigned char actLineNbr = 0;
	unsigned int i;
	
	unsigned char unicode_region=0;
	unsigned int  start_character=font->_font_unicode[0].start_character;
	unsigned int  font_info_i;
	unsigned char width;
	unsigned int height;
	unsigned int h=0;
	unsigned int start_index;
	unsigned char page;
	//unsigned int totalHeight;
	//unsigned int totalHeight_uint;
	//unsigned char currentY_uint;
	//int currentY_int;
//	unsigned int currentY;
	unsigned char ch;
	unsigned char mask;
	unsigned char p;
	unsigned int lcd_displaybuffer_address;
	
	int16_t actLineY;
	int16_t lineY_pix;
	uint8_t lineY_pixU8=0;
	uint8_t X;
	
	//if(pix_offset > windowHeight) // jezeli tekst jest poza obszarem widzenia, nie rysowac
	//		return;
	
	getDivdeCharIndexes(c, resultIndexArray, resultLenArray, &resLinesNbr, windowWidth, font);
	
	if( (resLinesNbr) < ( (windowHeight/font->height) + 1) ) pix_offset = 0;// dont scroll when string fit to area
	/// liczenie pierwszej linii
	actLineY = (Ystart + windowHeight + pix_offset) - font->height;
	// end liczenie pierwszej lini

	
	if(mode & 0x02)
	{
		X =  Xstart + ( (windowWidth - resultLenArray[0])>>1) + 1;
	}
	else
	{
		X = Xstart;
	}
	
	for(i = 0; c[i];)
	{
		if( (actLineNbr < resLinesNbr) && (i == resultIndexArray[actLineNbr]) )
		{
			actLineY -= font->height;
			actLineNbr++;
			
			if( ( (actLineY + font->height) < (signed)Ystart) )
			{
				break; // stop jezeli czubek litery poza V.A.
			}
			
			if(mode & 0x02)
			{
				X = Xstart + ( (windowWidth - resultLenArray[actLineNbr])>>1) + 1;
			}
			else
			{
				X = Xstart;
			}
		}
		
		if (c[i] == 9)
		{
      i++;
      unicode_region = font->_font_unicode[ c[i] - 48].unicode_region;
      start_character = font->_font_unicode[ c[i] - 48].start_character - font->_font_unicode[ c[i] - 48].offset;
      i++;
    }
    if (c[i] != 32)
		{
			font_info_i = (( (unicode_region << 8) | (c[i]) ) - start_character);
	
			start_index = font->_font_info[ font_info_i ].index;
			width = font->_font_info[ font_info_i ].width;
			height = font->_font_info[ font_info_i ].height;
		
		

		for(h = 0; h < height; h++)
		{
			lineY_pix = actLineY + h;
 			if(lineY_pix > ((signed)Ystart + windowHeight) ) break;
 			//if( lineY_pix < Ystart ) continue;
			lineY_pixU8 = 0xFF & lineY_pix;
			//if( lineY_pixU8 > ((signed)Ystart + windowHeight) ) break;
			//if( lineY_pixU8 < Ystart ) continue;
			
			page = (lineY_pixU8) >> 3;///8;
			ch=font->_font_table[start_index++];
			mask = 0x80;
			
			for(p = 0; p < width; p++)
			{
				if ( ((p%8)==0 ) && (p!=0)  )				        //jezeli liczba kolumn wieksza niz 8 pobirz kolejny bajt		
				{
					ch=font->_font_table[start_index++];
					mask=0x80;
				}
				lcd_displaybuffer_address=(page * 128) + X + p;
				
        if (lcd_displaybuffer_address>=1024)
				{
					lcd_displaybuffer_address=9999;   //for breakpoint test only
					return; 
        }
				
				if(mode&0x01)
				{
					if (ch&mask)
					{
						lcd_displaybuffer[lcd_displaybuffer_address]  |=   (1<< ((lineY_pixU8) % 8));
					}								
					else
					{
						lcd_displaybuffer[lcd_displaybuffer_address]  &= ~(1 <<((lineY_pixU8) % 8));
					}
				}
				else
				{
					if (ch&mask) 	
					{
						lcd_displaybuffer[lcd_displaybuffer_address] &= ~(1<< ((lineY_pixU8) % 8));
					}								
					else 
					{
						lcd_displaybuffer[lcd_displaybuffer_address] |=   (1 <<((lineY_pixU8) % 8));
					}
				}
				mask >>=1;//=mask/2;
			}
		}
		if (c[i+1]!=0) 
			rectangle(X+width,lineY_pixU8,1,font->soft_space,!mode);       //odstep pomiedzy kolejnymi znakami  
		
		X += width+font->soft_space;//=X+width+font->soft_space;    
		}     
		else
		{
			rectangle(X,lineY_pixU8,1,font->hard_space,!mode);		        //wyswietl znak spacji    
			X += font->hard_space;//=X+font->hard_space;
		}
		
// 		if( i == resultIndexArray[actLineNbr] )
// 		{
// 			if(currentY >= font->height)
// 			{
// 				currentY -= font->height;
// 				Xstart = 0;
// 				actLineNbr++;
// 			}
// 			else
// 				currentY = Ystart;
//  				Xstart = 0;
// 				actLineNbr++;
// 		}
		i++;
			//lcd_updatewholedisplay();
	}
}

void displayTextScroll_unicode(unsigned char Xstart, unsigned char Ystart,unsigned int pix_offset,unsigned char window,const char *c,const struct FONT_TYPE *font,unsigned char mode)
{
  unsigned char X=Xstart;  
  unsigned char X_end=Xstart+window;
  unsigned char x=0;
  
  unsigned char h,ch,p,page,mask,width,byte_width,__space;
  unsigned int start_index,index,height;
  unsigned int lcd_displaybuffer_address;
	
	unsigned char unicode_region=0;
	unsigned int  font_info_i;
	unsigned int  start_character=font->_font_unicode[0].start_character;
  
  while(*c)
	{
		if ((*c) == 9)
		{
      c++;
      unicode_region = font->_font_unicode[*c - 48].unicode_region;
      start_character = font->_font_unicode[*c - 48].start_character - font->_font_unicode[*c - 48].offset;
      c++;
    }
    if ((*c)!=32)
		{
			font_info_i = (( (unicode_region << 8) | (*c) ) - start_character);
	
			start_index = font->_font_info[ font_info_i ].index;
			width = font->_font_info[ font_info_i ].width;
			height = font->_font_info[ font_info_i ].height;
			
      byte_width = width >> 3;// / 8;
      if (width % 8) byte_width++;
      
      if (pix_offset > width)                                                    //Sprawdz czy pominac rysowanie 
      {  
				pix_offset -= width;
      }
      else                                                                    //rysuj literke
      {
        for (h = 0; h < height; h++)  	                                        //po kolejnych wierszach 	
        {  
					page = (Ystart+h) >> 3;///8;
          index = start_index + (h * byte_width) + (pix_offset / 9);
          ch = font->_font_table[index++];	
          mask = (0x80 >> (pix_offset % 8));
          x = 0;
 
          for (p=pix_offset; p<width; p++)  	                                // po kolejnych kolumnach				
          {   
             if ( ((p%8)==0 ) && (p!=0)  ){				        //jezeli liczba kolumn wieksza niz 8 pobirz kolejny bajt		
                ch=font->_font_table[index++];						
                mask=0x80; 
             }			 
                                                                                // zapisz pixel do pamieci    
             if (X+x>X_end) break;                                              //jezeli nie mieszcze sie w oknie to nastepny wiersz
             lcd_displaybuffer_address=(page << 7/* *128*/) + X + x;
             if (lcd_displaybuffer_address >= 1024){
               lcd_displaybuffer_address = 9999;   //for breakpoint test only
               return; 
             }
             if(mode){  
                if (ch&mask) 	
								{
									lcd_displaybuffer[lcd_displaybuffer_address]  |=   (1<< ((Ystart+h) % 8));
								}            		  
                else	
								{									
									lcd_displaybuffer[lcd_displaybuffer_address] &= ~ (1 <<((Ystart+h) % 8)); 
								}
             } 
             else{                               
                if (ch&mask) 	
								{
									lcd_displaybuffer[lcd_displaybuffer_address]  &=~  (1<< ((Ystart+h) % 8));
								}									
                else
								{
									lcd_displaybuffer[lcd_displaybuffer_address] |=   (1 <<((Ystart+h) % 8)); 
								}
             }
             x++;
             //lcd_updatewholedisplay(); //for test only                              
             mask >>= 1 ;//=mask/2;	
          }
        }
        pix_offset=0;                                                           //od tego momentu normalne wyswietlanie bez offsetu
      }  
      
      X += x;
      if (X > X_end) return;
      
      if (*(c+1) != 0){
        if (pix_offset > font->soft_space) pix_offset -= font->soft_space;
        else
					{
          __space=font->soft_space - pix_offset;
          
          if (X + __space > X_end){ 
            rectangle(X,Ystart,font->height,X_end - X + 1,!mode);		        //wyswietl znak spacji
            return;
          }
          
          rectangle(X,Ystart,height,__space,!mode);                          //odstep pomiedzy kolejnymi znakami
          X += __space; // = X + __space;
          pix_offset = 0;
        }
      }
      
    }//------------------------rysuj twarda spacje------------------------------
    else {
      if (pix_offset > font->hard_space) pix_offset -= font->hard_space;
        else
				{
          __space = font->hard_space - pix_offset;
          
          if (X+ __space > X_end){ 
            rectangle(X, Ystart, font->height, X_end - X + 1, !mode);		        //wyswietl znak spacji
            return;
          }
          
          rectangle(X, Ystart, font->height, __space, !mode);		        //wyswietl znak spacji
          X += __space;//=X+__space;
          pix_offset = 0;
        }
    }
    c++;                                                                        //nastepny znak    
  }	
}

//------------------------------------------------------------------------
void lcd_io_init(void){
		  
    GPIO_InitTypeDef GPIO_InitStructure;
    __GPIOA_CLK_ENABLE();
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |  RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitStructure.Pin =  LCD_SI | LCD_SCL | LCD_A0 | LCD_CS | LCD_RST; //|LCD_LIGHT;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LCD_CTRL_PORT, &GPIO_InitStructure);
   // LCD_CTRL_PORT->BSRRL  |= (LCD_SI | LCD_SCL | LCD_A0 | LCD_CS | LCD_RST);//|LCD_LIGHT);
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//------------------------------------------------------------------------
void lcd_updatewholedisplay(uint8_t is_invert) {
  unsigned char page,col;

  LCD_A0_DATA &= ~LCD_A0;
  if(is_invert == 1) 
  {
  	lcd_sendspibyte(0xA1); /* Reverse display */ //d4
  	lcd_sendspibyte(0xC8); /* select common output 0 - 63 */ //d4
	}
  else
  {
  	lcd_sendspibyte(0xA0); /* Reverse display */	 //moje
  	lcd_sendspibyte(0xC0); /* select common output 0 - 63 */	//moje
  }

  for (page = 0; page < 8; page++) {
    LCD_A0_DATA &= ~LCD_A0;
    /* Select page */
    lcd_sendspibyte(0xB0 | page);
    /* Jump to column 0 */
    lcd_sendspibyte(0x10);
    lcd_sendspibyte(0x00);
//lcd_sendspibyte(0x1f);
//    lcd_sendspibyte(0x0f);
    LCD_A0_DATA |= LCD_A0;
    for (col = 0; col < 128; col++) 
//	for (col = 127; col >= 0; col--)
	{
      lcd_sendspibyte(lcd_displaybuffer[page * 128 + col]);
    }
  }

  
}
//------------------------------------------------------------------------
void lcd_sendspibyte(unsigned char v) {
  unsigned char i;
  for (i = 0; i < 8; i++) {
    if (v & 0x80)     LCD_SI_DATA |=  LCD_SI;
    else 	      LCD_SI_DATA &= ~LCD_SI;
    
    LCD_SCL_DATA |=   LCD_SCL;    /* pull clock line high. Slave will read data here! */
    LCD_SCL_DATA &=  ~LCD_SCL; 	  /* Take back SCL. */
    v <<= 1;
  }
}
//------------------------------------------------------------------------
void lcd_init(void) {
  lcd_io_init();
	//LCD_LIGHT_DATA |= LCD_LIGHT; // set ligh on 
  LCD_RST_DATA|= LCD_RST;
  LCD_CS_DATA |= LCD_CS;
  _delay_ms(100.0); 
  LCD_CS_DATA &= ~LCD_CS;
  //------------------------------------------------------
  lcd_sendspibyte(0x40); /* Start line 0 */
  lcd_sendspibyte(0xA0); /* Reverse display */	 //moje
 // lcd_sendspibyte(0xA1); /* Reverse display */ //d4
  lcd_sendspibyte(0xC0); /* select common output 0 - 63 */	//moje
 //lcd_sendspibyte(0xC8); /* select common output 0 - 63 */ //d4
  lcd_sendspibyte(LCD_notinverted); /* Display normal not inverted */
  lcd_sendspibyte(0xA2); /* BIAS 1/9  Duty 1/65 */
  lcd_sendspibyte(0x2F); /* Booster, regulator and follower on */
  lcd_sendspibyte(0xF8); /* set internal booster 4x */
  lcd_sendspibyte(0x00); /* ...cont. */
  lcd_sendspibyte(0x27); /* Voltage regulator set */
  lcd_sendspibyte(0x81); /* contrast set */
  lcd_sendspibyte(LCD_contrast); /* ...cont. */
  lcd_sendspibyte(0xAC); /* Static indicator set 0 */
  lcd_sendspibyte(0x00); /* ...cont. */
  lcd_sendspibyte(0xAF); /* Display on */
  lcd_updatewholedisplay(0);
}

void lcd_turnOff(void)
{
	LCD_A0_DATA &= ~LCD_A0;
	lcd_sendspibyte(0xAE); /* Display off */
	LCD_A0_DATA |= LCD_A0;
}


// wyliczenie pozycji napisu zeby byl wyrownany do srodka zakresu startX, stopX
uint8_t getXpositionCenteredText_unicode(uint8_t startX, uint8_t stopX, char *c_ptr, const struct FONT_TYPE *font_ptr)
{
	uint8_t pos = 0;
	uint8_t width_str=0, width_line = 0;
	uint16_t i;
	
	unsigned char unicode_region = 0;
	unsigned int  font_info_i;
	unsigned int  start_character = font_ptr->_font_unicode[0].start_character;

	if(startX>stopX) return 0;
	width_line = stopX - startX;


	for(i=0;c_ptr[i];i++)
	{
		if( c_ptr[i] == 9 )
		{
			i++;
			unicode_region = font_ptr->_font_unicode[ c_ptr[i] -48].unicode_region;
      start_character= font_ptr->_font_unicode[ c_ptr[i] -48].start_character - font_ptr->_font_unicode[ c_ptr[i] -48].offset;
			i++;
		}
		if(c_ptr[i]!=32)
		{
			font_info_i=(((unicode_region<<8)|( c_ptr[i]) )-start_character);
			
			width_str += font_ptr->_font_info[ font_info_i ].width;
			if(c_ptr[i+1])width_str+= font_ptr->soft_space;
		}
		else
			width_str+= font_ptr->hard_space;
	}

	if(width_line < width_str)
		return 0;

	pos = (uint8_t) ( (width_line - width_str)/2);
	pos++;
	pos+= startX;

	return pos;
}
/*uint8_t getTextWidth(char *c_ptr, const struct FONT_TYPE *font_ptr)
{
	
	uint8_t width_str=0,i;
	for(i=0;c_ptr[i];i++)
	{
		if(c_ptr[i]!=32)
		{
			width_str += font_ptr->_font_info[ (c_ptr[i])-font_ptr->start_character ].width;
			if(c_ptr[i+1])width_str+= font_ptr->soft_space;
		}
		else
			width_str+= font_ptr->hard_space;
	}
	return width_str;
	
	return 0;
}*/

uint16_t getStringLength_pixels_unicode(char *c_in, const struct FONT_TYPE *font_ptr)
{
	uint16_t len_str=0,i;
	
	unsigned char unicode_region = 0;
	unsigned int  font_info_i;
	unsigned int  start_character = font_ptr->_font_unicode[0].start_character;

	for(i=0;c_in[i];i++)
	{
		if( c_in[i] == 9 )
		{
			i++;
			unicode_region = font_ptr->_font_unicode[ c_in[i] -48].unicode_region;
      start_character= font_ptr->_font_unicode[ c_in[i] -48].start_character - font_ptr->_font_unicode[ c_in[i] -48].offset;
			i++;
		}
		
		if(c_in[i]!=32)
		{
			font_info_i=(((unicode_region<<8)|( c_in[i]) )-start_character);
			
			len_str += font_ptr->_font_info[ font_info_i ].width;
			if(c_in[i+1])len_str+= font_ptr->soft_space;
		}
		else
		{
			len_str+= font_ptr->hard_space;
		}
	}

	return len_str;
	
}

uint8_t getDivdeCharIndexes(const char *c_in, unsigned int *resultIndexArray_ptr, unsigned char *resultLenArray_ptr, unsigned char *resultLinesNbr, unsigned char line_width, const struct FONT_TYPE *font_ptr)
{
	unsigned int i;
	unsigned char line_cnt = 0;
	
	unsigned char unicode_region = 0;
	unsigned int  font_info_i;
	unsigned int  start_character = font_ptr->_font_unicode[0].start_character;
	unsigned int len_str;
	
	unsigned int last_space_index = 0;
	unsigned char len_before_last_space = 0;
	
	len_str = 0;
	for(i = 0; c_in[i]; i++)
	{
		if( c_in[i] == 9 )
		{
			i++;
			unicode_region = font_ptr->_font_unicode[ c_in[i] - 48].unicode_region;
      start_character= font_ptr->_font_unicode[ c_in[i] - 48].start_character - font_ptr->_font_unicode[ c_in[i] -48].offset;
			i++;
		}
		
		if(c_in[i]!=32)
		{
			font_info_i=(((unicode_region << 8)|( c_in[i]) ) - start_character);
			
			len_str += font_ptr->_font_info[ font_info_i ].width;
			if( c_in[i+1] )len_str += font_ptr->soft_space;
			
			if(len_str > (line_width-font_ptr->hard_space) )
			{
				if(last_space_index == 0)
				{
					resultIndexArray_ptr[line_cnt] = i;// - 1;
					resultLenArray_ptr[ line_cnt ] = len_str - font_ptr->_font_info[ font_info_i ].width;
					len_str = font_ptr->_font_info[ font_info_i ].width;
				}
				else
				{
					resultIndexArray_ptr[line_cnt] = last_space_index;// - 1;
					resultLenArray_ptr[line_cnt] = len_before_last_space - font_ptr->_font_info[ font_info_i ].width;
					len_str = 0;
					i = last_space_index + 1;
					last_space_index = 0;
				}
				line_cnt++;
			}
		
		}
		else
		{
			len_str += font_ptr->hard_space;
			last_space_index = i;
			len_before_last_space = len_str;
		}
	}
	resultLenArray_ptr[line_cnt] = len_str;
	*resultLinesNbr = line_cnt;
	
	return 0;
}

uint8_t cutTextToWidth_unicode(char *c_in, char *c_out, uint8_t width, const struct FONT_TYPE *font_ptr, uint8_t cutToLastSpaceFlag) // if 1 == caly napis 0==przerwano
{
	
	uint8_t width_str=0,i, last_space_index=0;
	uint8_t is_wholeStr=1;

	unsigned char unicode_region = 0;
	unsigned int  font_info_i;
	unsigned int  start_character = font_ptr->_font_unicode[0].start_character;
	
	for(i=0;c_in[i];i++)
	{
		if( c_in[i] == 9 )
		{
			c_out[i] = c_in[i];
			i++;
			c_out[i] = c_in[i];
			unicode_region = font_ptr->_font_unicode[ c_in[i] - 48].unicode_region;
      start_character= font_ptr->_font_unicode[ c_in[i] - 48].start_character - font_ptr->_font_unicode[ c_in[i] -48].offset;
			i++;
		}
		
		if(c_in[i]!=32)
		{
			font_info_i=(((unicode_region<<8)|( c_in[i]) )-start_character);
			
			width_str += font_ptr->_font_info[ font_info_i].width;
			if(c_in[i+1])width_str+= font_ptr->soft_space;
		}
		else
		{
			width_str+= font_ptr->hard_space;
			last_space_index = i;
		}

		if(width < width_str) 
		{
			if(last_space_index && cutToLastSpaceFlag)i = last_space_index;
			is_wholeStr =0;
			break;
		}
		c_out[i]=c_in[i];
	}
	c_out[i]=0;
	return is_wholeStr;
	
	//return 0;
}

uint8_t divdeStringToNLines_unicode(char *c_in, char **lines, uint8_t width,const struct FONT_TYPE *font_ptr, uint8_t line_no, uint8_t cutToLastSpaceFlag)
{
	uint8_t status,i,len;
	uint8_t additionalChars = 0;
	char *temp = c_in;
	char *temp2;
	char *lastpageID_ptr;
	char char_tab[2];
	//char char_temp_buf[128];
	
	for(i=0;i<line_no;)
	{
		status = cutTextToWidth_unicode(temp, lines[i]+additionalChars,width, font_ptr, cutToLastSpaceFlag);//1);
		if(status) {i++;break;}
		temp2 = strrchr(lines[i],' ');
		if(temp2)*temp2 = 0;
		len = strlen(lines[i]);
		if(additionalChars)
		{
			len -= additionalChars;
			additionalChars = 0;
		}
		temp += (len+1);
		
		lastpageID_ptr = strrchr(lines[i],'\t');
		if( (lastpageID_ptr != 0) && ( (i+1) < line_no) )
		{
			char_tab[0] = *lastpageID_ptr;
			char_tab[1] = *(lastpageID_ptr+1);
			
			strcpy(lines[i+1],char_tab);
			additionalChars = 2;
		}
		i++; 
	}
	return i;
}



void displayText_GLCD_v_unicode(unsigned char X, unsigned char Y,char *c,const struct FONT_TYPE *font)
{
	unsigned char h,ch,width,w,w2;
	uint8_t char_temp=0;
	unsigned int index,height, w_B;
	uint32_t element_size=0,i;
	
	unsigned char unicode_region=0;
	unsigned int  font_info_i;
	unsigned int  start_character=font->_font_unicode[0].start_character;
	
	w=0;h=0;
	while(*c)
	{
		if ((*c)==9)
		{
      c++;
      unicode_region= font->_font_unicode[*c-48].unicode_region;
      start_character=font->_font_unicode[*c-48].start_character-font->_font_unicode[*c-48].offset;
      c++;
    }
		if ((*c)!=32)
		{
			font_info_i=(((unicode_region<<8)|(*c) )-start_character);
			
			index=font->_font_info[ font_info_i].index;
			width=font->_font_info[ font_info_i ].width;
	    height=font->_font_info[ font_info_i ].height;

			w_B = width >> 3; ///8;
			if( ((double)width/8) > 0.0) w_B++;

			element_size = w_B*height;

			for(i=0,w=0,h=0;i<element_size;i++)
			{
				if(  ((i% (w_B) )==0) &&i!=0 ) {w=0;h++;}
				ch=font->_font_table[index++];
				
			w2= w + (X >> 3);////w+X/8;
			
			char_temp = lcd_displaybuffer[128*(7-w2) + Y+h];
	        lcd_displaybuffer[128*(7-w2) + Y+h]=0;
			lcd_displaybuffer[128*(7-w2) + Y+h] |= (char_temp&(0xff<<(8-(X%8)))) | (ch>>(X%8)) ;
			lcd_displaybuffer[128*(7-w2-1) + Y+h] &=  ~(0xff<<(8-(X%8)));
			lcd_displaybuffer[128*(7-w2-1) + Y+h] |= ch<<(8-(X%8)); 
			
				//lcd_displaybuffer[128*(7-w) + h] = ch;

				w++;
			}
		//	if (*(c+1)!=0)rectangle(X+width,Y,height,font.soft_space,0); //odstep pomiêdzy kolejnymi znakami	  //komentowane
			X += width+font->soft_space; //=X+width+font->soft_space;
			
		}
		else {
		//	rectangle(X,Y,font.height,font.hard_space,0);		//wyswietl znak spacji		//koment
			X += font->hard_space; //=X+font->hard_space;
		}
		c++;
	}	
}

void displayText_unicode(unsigned char X, unsigned char Y,const char *c,const struct FONT_TYPE *font,unsigned char mode)
{
  
unsigned char h,ch,p,page,mask,width;
unsigned int index,height;
unsigned int lcd_displaybuffer_address;

unsigned char unicode_region=0;
unsigned int  font_info_i;
unsigned int  start_character=font->_font_unicode[0].start_character;

  while(*c)
	{
    if ((*c)==9)
		{
      c++;
      unicode_region= font->_font_unicode[*c-48].unicode_region;
      start_character=font->_font_unicode[*c-48].start_character - font->_font_unicode[*c-48].offset;
      c++;
    }
		
    if ((*c)!=32)
		{
			font_info_i=(((unicode_region<<8)|(*c) )-start_character);
	
			index=font->_font_info[ font_info_i ].index;
			width=font->_font_info[ font_info_i ].width;
			height=font->_font_info[ font_info_i ].height;

			for (h=0; h<height; h++)
			{   	                                        //po kolejnych wierszach 
				//page=(Y+h)>>3;///8;
				page=(Y+h)/8;
				ch=font->_font_table[index++];
				mask=0x80;
	  
					for (p=0; p<width; p++)  	                                        // po kolejnych kolumnach
					{	
						if ( ((p%8)==0 ) && (p!=0)  )				        //jezeli liczba kolumn wieksza niz 8 pobirz kolejny bajt		
						{
							ch=font->_font_table[index++];
							mask=0x80;
						}	
					// zapisz pixel do pamieci
            lcd_displaybuffer_address=(page * 128) + X + p;
            if (lcd_displaybuffer_address>=1024)
						{
              lcd_displaybuffer_address=9999;   //for breakpoint test only
              return; 
            }
						
            if(mode)
						{
							if (ch&mask)
							{
								lcd_displaybuffer[lcd_displaybuffer_address]  |=   (1<< ((Y+h) % 8));
							}								
							else
							{
								lcd_displaybuffer[lcd_displaybuffer_address]  &= ~(1 <<((Y+h) % 8));
							}
						}
            else
						{
							if (ch&mask) 	
							{
								lcd_displaybuffer[lcd_displaybuffer_address] &= ~(1<< ((Y+h) % 8));
							}								
							else 
							{
								lcd_displaybuffer[lcd_displaybuffer_address] |=   (1 <<((Y+h) % 8));
							}
            }
            mask >>=1;//=mask/2;	
          }
        }

        if (*(c+1)!=0)	rectangle(X+width,Y,height,font->soft_space,!mode);       //odstep pomiedzy kolejnymi znakami  
        X += width+font->soft_space;//=X+width+font->soft_space;    
			}     
			else
			{
				rectangle(X,Y,font->height,font->hard_space,!mode);		        //wyswietl znak spacji    
				X += font->hard_space;//=X+font->hard_space;
			}    
			c++;
			//lcd_updatewholedisplay();
		}	
}
