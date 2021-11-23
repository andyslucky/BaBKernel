#pragma once
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define IDT_SIZE 256
struct IDT_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

#ifdef __cplusplus
extern "C" {
#endif

void load_idt(unsigned long *idt_ptr);

#ifdef __cplusplus
}
#endif

void idt_init(void);