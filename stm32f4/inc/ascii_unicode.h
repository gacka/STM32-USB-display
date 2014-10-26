
#ifndef __ASCII_H 
#define __ASCII_H

//#define RUSSION_FONTS_ACTIVE

#define WOODCAP_LOGO // Define CAISSON_LOGO, STORCH_LOGO, WOLF_LOGO, RADKE_LOGO, WOODCAP_LOGO, WITTE_LOGO, STAUF_LOGO, ROLL_LOGO or ELCOMETER_LOGO

#define hand_icon            "0"
#define arrow_circle_icon    "1"
#define battery_icon         "2"
#define power_icon           "3"
#define small_battery_icon	 4
#define small_battery_icon_v 63
#define LOW_BAT					     "C"
#define BALL_ICO				     "D"
#define BALL_V_ICO				   "E"
#define KEY_ICO				       "F"
#define KEY_V_ICO			   	   "G"
#define TQC_logo             "8"
#define Caisson_logo         "9"
#define Caisson_gothic       ":"
#define caisson_scale        ";"
 
 /// FONT_INDEX
#define CAMBRIA12PT		  0
#define LEELAWADEE_7PT	1
#define AHARONI9PT		  2
#define LEELAWADEE7PT 	3
#define MEIRYO_UI_INDEX	4
#define MEIRYO_UI2_IND	5
#define SYMBOL			    6
#define ICON			      7
#define CAMBRIA9PT		  8
#define CAMBRIA13PT	    9

struct FONT_TYPE {
	unsigned char height;
	unsigned char soft_space;
	unsigned char hard_space;	
	//char		        start_character;
	const unsigned char* 		_font_table;
	const struct FONT_CHAR_INFO* 	_font_info;
	const struct UNICODE_CHAR_INFO* _font_unicode;
};


struct FONT_CHAR_INFO {
	      unsigned char width;
        unsigned char height;
	      unsigned int index;
};

struct UNICODE_CHAR_INFO 
{
	unsigned char unicode_region;
  unsigned int  start_character;
	unsigned int  offset;
};

extern const struct UNICODE_CHAR_INFO generalUNICODE[];

// Font data for Cambria 12pt
extern const unsigned char cambria12ptBitmaps[];
extern const struct FONT_CHAR_INFO cambria12ptDescriptors[];
extern const struct FONT_TYPE cambria12pt;

extern const struct FONT_TYPE cambria9pt;

extern const struct FONT_TYPE leelawadee_7pt;
extern const struct FONT_TYPE meiryo_ui;
extern const struct FONT_TYPE MEIRYO_UI;

extern const unsigned char 		icon_CharBitmaps[];
extern const struct FONT_CHAR_INFO 	icon_CharDescriptors[];
extern const struct FONT_TYPE 		icon;

extern const unsigned char 		      cambria13ptCharBitmaps[];
extern const struct FONT_CHAR_INFO 	cambria13ptCharDescriptors[];
extern const struct FONT_TYPE 		  cambria13pt;

#endif /* end __ASCII_H */
