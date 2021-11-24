#include "screen.h"
#include <io/io_ports.h>


ScreenBufferChar* frame_buffer_bp = (ScreenBufferChar*)FRAME_BUFFER_START;

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

inline ScreenBufferChar* get_frame_buffer_offset(int row, int col) {
	return frame_buffer_bp + (row*SCREEN_CHARS_PER_LINE+col);

}

static void _move_cursor_to_pos(Point p){
	unsigned short pos = p.row * SCREEN_CHARS_PER_LINE + p.col;
	write_port(FB_CMD_PORT,FB_HIGHBYTE);
	write_port(FB_DATA_PORT,((pos >> 8) & 0x00FF));
	write_port(FB_CMD_PORT,FB_LOWBYTE);
	write_port(FB_DATA_PORT,pos & 0x00FF);
}

void ScreenBuffer::move_cursor(int row, int col){
	if(row < NUM_LINES && col < SCREEN_CHARS_PER_LINE){
		this->char_at_cursor = get_frame_buffer_offset(row,col);
		this->cursor_pos.row = row;
		this->cursor_pos.col = col;
		_move_cursor_to_pos(this->cursor_pos);
	}
}

ScreenBuffer::ScreenBuffer(){
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

void ScreenBuffer::set_char_at_pos(char c, int row,  int col) {
	ScreenBufferChar* screen_char = get_frame_buffer_offset(row,col);
	screen_char->set_value(c);
	screen_char->set_color_pair(this->current_color);
}


void ScreenBuffer::clear(){
	this->char_at_cursor = frame_buffer_bp;
	for(int row = 0; row < NUM_LINES; row++) {
		for(int col = 0; col < SCREEN_CHARS_PER_LINE; col++) {
			ScreenBufferChar* screen_char = get_frame_buffer_offset(row,col);
			screen_char->set_value(' ');
			screen_char->set_color_pair(ColorPair{Color::Black, Color::White});
		}
	}
	this->move_cursor(0,0);
}


ScreenBufferChar::ScreenBufferChar(char value, ColorPair color_pair) {
	this->character_value = value;
	this->encoded_color = encodeColor(color_pair);
}

ColorPair ScreenBufferChar::get_color_pair() {
	return decodeColor(this->encoded_color);
}

Color ScreenBufferChar::get_bg() {
	return decodeColor(this->encoded_color).bg;
}

Color ScreenBufferChar::get_fg() {
	return decodeColor(this->encoded_color).fg;
}

char ScreenBufferChar::get_value() {
	return this->character_value;
}

void ScreenBufferChar::set_value(char c) {
	this->character_value = c;
}

void ScreenBufferChar::set_color_pair(ColorPair color_pair) {
	this->encoded_color = encodeColor(color_pair);
}