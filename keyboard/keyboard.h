#ifndef KEYBOARD_H
#define KEYBOARD_H
extern void idt_init(void);
void kb_init(void);
extern char getch();
extern char* gets();
#endif