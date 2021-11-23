/*
*  kernel.c
*/
#include "screen/screen.h"
#include "keyboard/keyboard.h"

#ifdef __cplusplus
extern "C" {
#endif
void idle(void);

void kmain(void)
{
	ScreenBuffer screen_handle;
	idt_init();
	kb_init();
	screen_handle.set_color(ColorPair{Color::Black,Color::Red});
	screen_handle.set_char_at_pos('A',0,0);
	idle();
}

#ifdef __cplusplus
}
#endif