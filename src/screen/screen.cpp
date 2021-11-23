#include "screen.h"


#ifdef __cplusplus
extern "C" {
#endif

char read_port(unsigned short port);

void write_port(unsigned short port, unsigned char data);

#ifdef __cplusplus
}
#endif


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

static unsigned short constexpr get_frame_buffer_offset(int row, int col) {
	return row*BYTES_PER_LINE + col*sizeof(ScreenBufferChar);

}
static void _move_cursor_to_pos(Point p){
	// unsigned short pos = get_frame_buffer_offset(p.row, p.col);
	unsigned short pos = p.row * SCREEN_CHARS_PER_LINE + p.col;
	write_port(FB_CMD_PORT,FB_HIGHBYTE);
	write_port(FB_DATA_PORT,((pos >> 8) & 0x00FF));
	write_port(FB_CMD_PORT,FB_LOWBYTE);
	write_port(FB_DATA_PORT,pos & 0x00FF);
}

void ScreenBuffer::move_cursor(int row, int col){
	if(row < NUM_LINES && col < SCREEN_CHARS_PER_LINE){
		this->char_at_cursor = (ScreenBufferChar*)(FRAME_BUFFER_START + get_frame_buffer_offset(row,col));
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

void ScreenBuffer::set_char_at_pos(char c, int row,  int col) {
	this->move_cursor(row,col);
	*(this->char_at_cursor) = ScreenBufferChar{c, encodeColor(this->current_color)};
}

void ScreenBuffer::set_char_at_pos(ScreenBufferChar c, int row, int col) {
	this->move_cursor(row,col);
	*(this->char_at_cursor) = c;
}

void ScreenBuffer::clear(){
	this->char_at_cursor = (ScreenBufferChar*)FRAME_BUFFER_START;
	int offset = 0;
	for(offset = 0; offset < SCREEN_CHARS_PER_LINE * NUM_LINES; offset++){
		this->char_at_cursor[offset] = ScreenBufferChar{' ', encodeColor(this->current_color)};
	}
	this->move_cursor(0,0);
}