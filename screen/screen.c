#include "screen.h"
#define VIDEO_BP 0xb8000
#define CHARS_PER_LINE 160
#define NUM_LINES 25
#define BYTE_PER_CHAR 2
#define COLUMNS 80
#define VIDEO_MAP_SIZE 4000

#define MAX_SCREEN_CHARS 2000

#define FB_CMD_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
#define FB_HIGHBYTE 14
#define FB_LOWBYTE 15

extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
typedef struct {
	char val;
	char color;
} SCREEN_CHAR;


typedef struct _SCREEN{
	SCREEN_CHAR* bp;
	SCREEN_CHAR* current_ch;
	char colorCode;
	POS currPos;
} SCREEN;

char encodeColor(COLOR c){
	char bg = (char)c.bg;
	char fg = (char)c.fg;
	return (bg << 4) | fg;
}

COLOR decodeColor(char encodedByte){
	COLOR c;
	c.bg = encodedByte >> 4;
	c.fg = (encodedByte << 4) >> 4;
	return c;
}

SCREEN screen;
void move_cursor(POS p){
	unsigned short pos = p.row*80+p.col;
	write_port(FB_CMD_PORT,FB_HIGHBYTE);
	write_port(FB_DATA_PORT,((pos >> 8) & 0x00FF));
	write_port(FB_CMD_PORT,FB_LOWBYTE);
	write_port(FB_DATA_PORT,pos & 0x00FF);
}

void moveTo(int row, int col){
	if(row < NUM_LINES && col < COLUMNS){
		screen.current_ch = screen.bp + (row*80 + col);
		screen.currPos.row = row;
		screen.currPos.col = col;
		move_cursor(screen.currPos);
	}
}

void init_screen(void){
	screen.bp = (SCREEN_CHAR*)VIDEO_BP;
	screen.current_ch = (SCREEN_CHAR*)VIDEO_BP;
	screen.currPos.row = 0;
	screen.currPos.col = 0;
	moveTo(screen.currPos.row,screen.currPos.col);
}

COLOR getColor(){
	return decodeColor(screen.colorCode);
}

int setScreenColor(COLOR c){
	screen.colorCode = encodeColor(c);
}
void putc(char val){
	if(screen.current_ch < (SCREEN_CHAR*)(VIDEO_BP+VIDEO_MAP_SIZE)){
		SCREEN_CHAR c = (SCREEN_CHAR){val,screen.colorCode};
		*(screen.current_ch) = c;
		//update the cursor position
		if(screen.currPos.col == COLUMNS - 1){
			//go to the begining of the next line!
			moveTo(screen.currPos.row + 1,0);
		}else{
			moveTo(screen.currPos.row, screen.currPos.col + 1);
		}
	}else{
		return;
	}
}
unsigned int getOffset(){
	unsigned int curr_addr = (int)(screen.current_ch);
	unsigned int bp_addr = (int)(screen.bp);
	unsigned int offset = curr_addr - bp_addr;
	return offset;
}

POS getPos(){
	return screen.currPos;
}
void delch(){
	if(screen.current_ch == screen.bp){
		return;
	}
	POS p = getPos();
	POS newP;
	if(p.col == 0 && p.row > 0){
		newP.row = p.row - 1;
		newP.col = COLUMNS - 1;
	}else{
		newP.col = p.col - 1;
		newP.row = p.row;
	}
	moveTo(newP.row,newP.col);
	putc(' ');
	moveTo(newP.row,newP.col);
}

void clear(){
	screen.current_ch = screen.bp;
	int offset = 0;
	for(offset = 0; offset < COLUMNS * NUM_LINES; offset++){
		screen.bp[offset] = (SCREEN_CHAR){' ',screen.colorCode};
	}
	//screen.current_ch = screen.bp;
	moveTo(0,0);
}

void print(const char* format){
	const char* str = format;
	while(*str != '\0' && screen.current_ch < (SCREEN_CHAR*)(VIDEO_BP+VIDEO_MAP_SIZE)){
		if(*str == '\n'){
			unsigned int offset = getOffset();
			offset = CHARS_PER_LINE - (offset % CHARS_PER_LINE);
			offset = offset / sizeof(SCREEN_CHAR);
			screen.current_ch += offset;
			moveTo(screen.currPos.row + 1,0);
			++str;
			continue;
		}
		putc(*str);
		++str;
	}
}
void println(const char* str){
	print(str);
	print("\n\0");
}