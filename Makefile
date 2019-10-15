ASM_SRC = kernel.asm
LINK_LD = linker.ld
C_SRC = math/math.c string/string.c keyboard/keyboard.c screen/screen.c kernel.c memory/kmem.c memory/memory.c
TARGET = kernel
ASM_O = kasm.o
C_O = $(OBJS_DIR)/screen.o $(OBJS_DIR)/kc.o $(OBJS_DIR)/keyboard.o $(OBJS_DIR)/string.o $(OBJS_DIR)/math.o $(OBJS_DIR)/kmem.o $(OBJS_DIR)/memory.o
OBJS_DIR = objects

link:compileC
	@ld -m elf_i386 -T linker.ld -o $(TARGET) $(OBJS_DIR)/$(ASM_O) $(C_O)

compileC: assemble
	@gcc -m32 -ffreestanding -c $(C_SRC)
	@mv screen.o objects/screen.o
	@mv kernel.o objects/kc.o
	@mv keyboard.o objects/keyboard.o
	@mv math.o objects/math.o
	@mv string.o objects/string.o
	@mv kmem.o objects/kmem.o
	@mv memory.o objects/memory.o

assemble:
	@nasm -f elf32 $(ASM_SRC) -o $(OBJS_DIR)/$(ASM_O)
clean:
	rm -rf $(OBJS_DIR)/*
	rm -rf $(TARGET)