/*
*  kernel.c
*/
#include <screen/screen.h>
#include <keyboard/keyboard.h>
#include <idt/idt.h>

#ifdef __cplusplus
extern "C" {
#endif
void idle(void);

void kmain(void)
{
	TextModeBuffer screen_handle;
	idt_init();
	kb_init();
	screen_handle.set_color(TextModeColorPair{TextModeColor::Black,TextModeColor::Red});
	screen_handle.set_char_at_pos('H',0,0);
	screen_handle.set_char_at_pos('E',0,1);
	screen_handle.set_color(TextModeColorPair{TextModeColor::Black,TextModeColor::Green});
	screen_handle.set_char_at_pos('Y',0,2);
	
	screen_handle.move_cursor(1,0);
	idle();
}

#ifdef __cplusplus
}
#endif