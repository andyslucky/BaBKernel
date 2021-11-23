#include "keyboard.h"
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08



#ifdef __cplusplus
extern "C" {
#endif
void keyboard_handler(void);

char read_port(unsigned short port);

void write_port(unsigned short port, unsigned char data);

void load_idt(unsigned long *idt_ptr);

#ifdef __cplusplus
}
#endif

// Incomplete key mapping
// static unsigned char keyboard_map[128] =
// {
//   0,
//   27,
//   '1',
//   '2',
//   '3',
//   '4',
//   '5',
//   '6',
//   '7',
//   '8',
//   '9',
//   '0',
//   '-',
//   '=',
//   '\b',	/* Backspace */
//   '\t',/* Tab */
//   'q',
//   'w',
//   'e',
//   'r',	/* 19 */
//   't',
//   'y',
//   'u',
//   'i',
//   'o',
//   'p',
//   '[',
//   ']',
//   '\n',	/* Enter key */
//   0,			/* 29   - Control */
//   'a',
//   's',
//   'd',
//   'f',
//   'g',
//   'h',
//   'j',
//   'k',
//   'l',
//   ';',	/* 39 */
//   '\'',
//   '`',
//   0,		/* Left shift */
//  '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
//   'm', ',', '.', '/',   0,				/* Right shift */
//   '*',
//     0,	/* Alt */
//   ' ',	/* Space bar */
//     0,	/* Caps lock */
//     0,	/* 59 - F1 key ... > */
//     0,   0,   0,   0,   0,   0,   0,   0,
//     0,	/* < ... F10 */
//     0,	/* 69 - Num lock*/
//     0,	/* Scroll Lock */
//     0,	/* Home key */
//     0,	/* Up Arrow */
//     0,	/* Page Up */
//   '-',
//     0,	/* Left Arrow */
//     0,
//     0,	/* Right Arrow */
//   '+',
//     0,	/* 79 - End key*/
//     0,	/* Down Arrow */
//     0,	/* Page Down */
//     0,	/* Insert Key */
//     0,	/* Delete Key */
//     0,   0,   0,
//     0,	/* F11 Key */
//     0,	/* F12 Key */
//     0,	/* All other keys are undefined */
// };

/* video memory begins at address 0xb8000 */

struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];


void idt_init(void)
{
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	/* populate IDT entry of keyboard's interrupt */
	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

	/*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x00);
	write_port(0xA1 , 0x00);

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);

	/* fill the IDT descriptor */
	idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;

	load_idt(idt_ptr);
}

void kb_init(void)
{
	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
	write_port(0x21 , 0xFD);
}


// char getch(){
// 	unsigned char status;
// 	char keycode;
// 	char ascii;
// 	/* write EOI */
	
// 	/* Lowest bit of status will be set if buffer is not empty */
// 	while(1){
// 		write_port(0x20, 0x20);
// 		status = read_port(KEYBOARD_STATUS_PORT);
// 		if (status & 0x01) {
// 			keycode = read_port(KEYBOARD_DATA_PORT);
// 			if(keycode >= 0){
// 				ascii = keyboard_map[(unsigned char) keycode];
// 			}
// 			break;
// 		}
// 	}
// 	return ascii;
// }
// char* gets(){
//   char c;
//   int size = 0;
//   char* str = malloc(1024);
//   while((c = getch()) != '\n'){
//     str[size++] = c;
//   }
//   str[size++] = '\0';
//   return str;
// }

#ifdef __cplusplus
extern "C" 
#endif
void keyboard_handler_main(void)
{

}