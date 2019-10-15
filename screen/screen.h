#ifndef SCREEN_H
#define SCREEN_H
enum COLORS {
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
typedef struct _COLOR{
	enum COLORS bg;
	enum COLORS fg;
} COLOR;

typedef struct _POS{
	unsigned int row;
	unsigned int col;
} POS;
extern void init_screen(void);

extern int setScreenColor(COLOR c);

extern void putc(char val);

extern void delch();
extern POS getPos();

extern void moveTo(int row, int col);

extern COLOR getColor();

extern void clear();

extern void print(const char* format);

extern void println(const char* str);

#endif