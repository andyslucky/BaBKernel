ASM_SRC = kernel.asm
LINK_LD = linker.ld
TARGET = kernel
ASM_O = kasm.o
OBJS_DIR = objects
dirs := math keyboard memory screen string
files := $(foreach dir,$(dirs),$(wildcard $(dir)/*.c))
objects = $(OBJS_DIR)/kasm.o $(OBJS_DIR)/kernel.o $(OBJS_DIR)/keyboard.o $(OBJS_DIR)/math.o $(OBJS_DIR)/memory.o $(OBJS_DIR)/screen.o $(OBJS_DIR)/string.o
link:compileC
	@ld -m elf_i386 -T linker.ld -o $(TARGET) $(objects)

compileC: assemble
	@gcc -m32 -ffreestanding -c kernel.c $(files)
	@mv *.o objects/
	

assemble:
	@nasm -f elf32 $(ASM_SRC) -o $(OBJS_DIR)/$(ASM_O)
clean:
	rm -rf $(OBJS_DIR)/*
	rm -rf $(TARGET)