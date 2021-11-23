#include "screen.h"


#ifdef __cplusplus
extern "C" 
#endif
char read_port(unsigned short port);

#ifdef __cplusplus
extern "C" 
#endif
void write_port(unsigned short port, unsigned char data);


char encodeColor(ColorPair c){
	char bg = (char)c.bg;
	char fg = (char)c.fg;
	return (bg << 4) | fg;
}

ColorPair decodeColor(char encodedByte){
	ColorPair c;
	c.bg = (Color)(encodedByte >> 4);
	c.fg = (Color)((encodedByte << 4) >> 4);
	return c;
}

// SCREEN screen;
static void _move_cursor_to_pos(Point p){
	unsigned short pos = p.row * COLUMNS + p.col;
	write_port(FB_CMD_PORT,FB_HIGHBYTE);
	write_port(FB_DATA_PORT,((pos >> 8) & 0x00FF));
	write_port(FB_CMD_PORT,FB_LOWBYTE);
	write_port(FB_DATA_PORT,pos & 0x00FF);
}

void ScreenBuffer::move_cursor(int row, int col){
	if(row < NUM_LINES && col < COLUMNS){
		this->char_at_cursor = (ScreenBufferChar*)(FRAME_BUFFER_START + (row*COLUMNS + col));
		this->cursor_pos.row = row;
		this->cursor_pos.col = col;
		_move_cursor_to_pos(this->cursor_pos);
	}
}

ScreenBuffer::ScreenBuffer(){
	this->char_at_cursor = (ScreenBufferChar*)FRAME_BUFFER_START;
	this->clear();
}

ColorPair ScreenBuffer::get_color(){
	return this->current_color;
}

void ScreenBuffer::set_color(ColorPair c){
	this->current_color = c;
}

Point ScreenBuffer::get_cusror_pos(){
	return this->cursor_pos;
}

void ScreenBuffer::set_char_at_pos(char c, int col, int row) {
	this->move_cursor(row,col);
	*(this->char_at_cursor) = ScreenBufferChar(c, this->current_color);
}

void ScreenBuffer::set_char_at_pos(ScreenBufferChar c, int col, int row) {
	this->move_cursor(row,col);
	*(this->char_at_cursor) = c;
}

void ScreenBuffer::clear(){
	this->char_at_cursor = (ScreenBufferChar*)FRAME_BUFFER_START;
	int offset = 0;
	for(offset = 0; offset < COLUMNS * NUM_LINES; offset++){
		this->char_at_cursor[offset] = ScreenBufferChar(' ',this->current_color);
	}
	//screen.current_ch = screen.bp;
	this->move_cursor(0,0);
}

ScreenBufferChar::ScreenBufferChar(char value, ColorPair color_pair){
	this->character_value = value;
	this->encoded_color = encodeColor(color_pair);
}