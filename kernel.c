/*
*  kernel.c
*/
#include "screen/screen.h"
#include "keyboard/keyboard.h"
#include "string/string.h"
#include "memory/memory.h"

//extern void sleep(short,short);
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

#define ENTER_KEY_CODE 0x1C

#define BKSP 0xe

#define LSHIFT 42

#define ESC 1



extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);
COLOR c;
void printLogo(){
	//setScreenColor(new);
	const char* logo = "||==\\           ||==\\\n"\
					   "||   \\          ||   \\\n"\
					   "||==||  |== +   ||==||\n"\
					   "||   /  |   ||  ||   /\n"\
					   "||==/   == ++++ ||==/  Andrew Strickland 2019.\n\0";
	println(logo);
	println("The 'BaB' kernel! Pronounced 'Baby' is a small kernel for POC. Try --help to get started!\0");
}
void (*overrideKeyboardHandler)(char);
int buff_pos;
char buff[255];
void clearBuff(){
	for(buff_pos = 0; buff_pos < 254; buff_pos++){
		buff[buff_pos] = 0;
	}
	buff_pos = 0;
}

void printHelp(){
	const char* help = "RECOGNIZED COMMANDS:\n"\
					   "clear : clears the screen\n"
					   "babt : Enters the BaB kernel text editor\n\0";
	print(help);
}

void babt(char keycode){
	// if(keycode == 1){
	// 	print("Exiting BaB text editor!\n\0");
	// 	overrideKeyboardHandler = 0x0;
	// 	return;
	// }else if(keycode == BKSP){
	// 	delch();
	// 	return;
	// }else if(keycode == ENTER_KEY_CODE) {
	// 	print("\n\0");
	// 	return;
	// }else{
	// 	char ascii = keyboard_map[(unsigned char) keycode];
	// 	putc(ascii);
	// }
}
char debug = 0;
void processCommand(){
	if(strcmp(buff,"clear\0") == 0){
		clear();
	}else if(strcmp(buff,"--help\0") == 0){
		printHelp();
	}else if(strcmp(buff,"babt\0") == 0){
		clear();
		print("Welcome to the BaB text editor! Hit escape when you would like to leave!\n\0");
		overrideKeyboardHandler = babt;
	}else if(strcmp(buff,"logo\0") == 0){
		clear();
		printLogo();
		return;
	}else if(strcmp(buff,"debug\0") == 0){
		debug = !debug;
		return;
	}else if(strcmp(buff,"color\0") == 0){
		c.bg++;
		c.fg++;
		setScreenColor(c);
		clear();
		return;
	}else{
		print("Unknown command was entered! Try --help for options!\n\0");
	}
}
extern void idle(void);
char* allocMystr(){
	char* mystr = malloc(1024);
	for(int i =0 ; i < 1024; i++){
		mystr[i] = i % 128 + 1;
	}
	return mystr;
}
void kmain(void)
{
	overrideKeyboardHandler = 0x0;
	clearBuff();
	init_screen();
	idt_init();
	kb_init();
	c.bg = Black;
	c.fg = Red;
	setScreenColor(c);
	clear();
	char* mystr = gets();
	print(mystr);
	
	idle();
	
}