#include "screen.h"
#include <io/io_ports.h>


TextModeBufferChar* frame_buffer_bp = (TextModeBufferChar*)TEXT_MODE_FRAME_BUFFER_START;

char encodeColor(TextModeColorPair c){
	char bg = (char)c.bg;
	char fg = (char)c.fg;
	return (bg << 4) | fg;
}

TextModeColorPair decodeColor(char encodedByte){
	TextModeColorPair c;
	c.bg = (TextModeColor)(encodedByte >> 4);
	c.fg = (TextModeColor)(encodedByte & 0x0f);
	return c;
}

inline TextModeBufferChar* get_frame_buffer_offset(int row, int col) {
	return frame_buffer_bp + (row * SCREEN_CHARS_PER_LINE + col);
}

static void _move_cursor_to_pos(Point* p){
	unsigned short pos = p->row * SCREEN_CHARS_PER_LINE + p->col;
	write_port(FB_CMD_PORT,FB_HIGHBYTE);
	write_port(FB_DATA_PORT,((pos >> 8) & 0x00FF));
	write_port(FB_CMD_PORT,FB_LOWBYTE);
	write_port(FB_DATA_PORT,pos & 0x00FF);
}

void TextModeBuffer::move_cursor(int row, int col){
	if(row < NUM_LINES && col < SCREEN_CHARS_PER_LINE){
		this->char_at_cursor = get_frame_buffer_offset(row,col);
		this->cursor_pos.row = row;
		this->cursor_pos.col = col;
		_move_cursor_to_pos(&this->cursor_pos);
	}
}

TextModeBuffer::TextModeBuffer(){
	this->clear();
	this->move_cursor(0,0);
}

TextModeColorPair TextModeBuffer::get_color(){
	return this->current_color;
}

void TextModeBuffer::set_color(TextModeColorPair c){
	this->current_color = c;
}

Point TextModeBuffer::get_cusror_pos(){
	return this->cursor_pos;
}

void TextModeBuffer::set_char_at_pos(char c, int row,  int col) {
	TextModeBufferChar* screen_char = get_frame_buffer_offset(row,col);
	screen_char->set_value(c);
	screen_char->set_color_pair(this->current_color);
}


void TextModeBuffer::clear(){
	for(int row = 0; row < NUM_LINES; row++) {
		for(int col = 0; col < SCREEN_CHARS_PER_LINE; col++) {
			TextModeBufferChar* screen_char = get_frame_buffer_offset(row,col);
			screen_char->set_value(' ');
			screen_char->set_color_pair(TextModeColorPair{TextModeColor::Black, TextModeColor::White});
		}
	}
}


TextModeBufferChar::TextModeBufferChar(char value, TextModeColorPair color_pair) {
	this->character_value = value;
	this->encoded_color = encodeColor(color_pair);
}

TextModeColorPair TextModeBufferChar::get_color_pair() {
	return decodeColor(this->encoded_color);
}

TextModeColor TextModeBufferChar::get_bg() {
	return decodeColor(this->encoded_color).bg;
}

TextModeColor TextModeBufferChar::get_fg() {
	return decodeColor(this->encoded_color).fg;
}

char TextModeBufferChar::get_value() {
	return this->character_value;
}

void TextModeBufferChar::set_value(char c) {
	this->character_value = c;
}

void TextModeBufferChar::set_color_pair(TextModeColorPair color_pair) {
	this->encoded_color = encodeColor(color_pair);
}