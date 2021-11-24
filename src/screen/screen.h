#pragma once

#define FRAME_BUFFER_START 0xb8000

#define NUM_LINES 25
#define BYTE_PER_SCREEN_CHAR 2
#define SCREEN_CHARS_PER_LINE 80
#define BYTES_PER_LINE SCREEN_CHARS_PER_LINE * BYTE_PER_SCREEN_CHAR
#define VIDEO_MAP_SIZE 4000

#define MAX_SCREEN_CHARS 2000

// Frame buffer 
#define FB_CMD_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
#define FB_HIGHBYTE 14
#define FB_LOWBYTE 15
enum class Color {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	Light_Grey = 7,
	Dark_Grey = 8,
	Light_Blue = 9,
	Light_Green = 10,
	Light_Cyan = 11,
	Light_Red = 12,
	Light_Magenta = 13,
	Light_Brown = 14,
	White = 15
};

typedef struct _ColorPair{
	enum Color bg;
	enum Color fg;
} ColorPair;

typedef struct _point{
	unsigned int row;
	unsigned int col;
} Point;

#pragma pack(1)
class ScreenBufferChar {
	private:
		char character_value;
		char encoded_color;
	public:
		ScreenBufferChar(char value, ColorPair color_pair);
		ColorPair get_color_pair();
		void set_color_pair(ColorPair color_pair);
		Color get_bg();
		Color get_fg();
		char get_value();
		void set_value(char c);
} ;

class ScreenBuffer {
	private:
		Point cursor_pos;
		ColorPair current_color;
		ScreenBufferChar* char_at_cursor;
	public:
		ScreenBuffer();

		void set_color(ColorPair c);

		Point get_cusror_pos();

		void move_cursor(int row, int col);

  		ColorPair get_color();

		void clear();

		/// Sets the character value 
		void set_char_at_pos(char c, int row, int col);

};