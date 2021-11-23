CPP_COMPILER = clang++
LINKER = ld.lld
ASSEMBLER = nasm


SRC_DIR = src
OBJS_DIR = objects

BOOTSTRAP_ASM = kernel.asm
LINKER_SCRIPT = linker.ld

ASSEMBLER_FLAGS = -f elf32
CPP_KERNEL_MAIN = kernel.cpp
CFLAGS = -m32 -target i386-pc-none-elf -Wall -ffreestanding -pedantic -nostdlib -nostdinc

TARGET = kernel
SRC_MODULES = $(foreach dir, $(SRC_DIR)/*,$(wildcard $(dir)/*.cpp))

QEMU_EMULATOR = qemu-system-i386
QEMU_FLAGS = -kernel

build:elf

elf:link
	@objcopy -O elf32-i386 $(TARGET) $(TARGET).elf
	@rm -rf $(TARGET)

link:compileC
	@$(LINKER) -T $(LINKER_SCRIPT) -o $(TARGET) objects/*
	

compileC: assemble
	@$(CPP_COMPILER) $(CFLAGS) -Isrc -c $(SRC_DIR)/$(CPP_KERNEL_MAIN) $(SRC_MODULES)
	@mv *.o $(OBJS_DIR)/

assemble: $(OBJS_DIR)
	@$(ASSEMBLER) $(ASSEMBLER_FLAGS) $(SRC_DIR)/$(BOOTSTRAP_ASM) -o $(OBJS_DIR)/$(TARGET)_asm.o

$(OBJS_DIR):
	@mkdir $(OBJS_DIR)

clean:
	@rm -rf $(OBJS_DIR)
	@rm -rf $(TARGET)
	@rm -rf $(TARGET).elf

run:
	@$(QEMU_EMULATOR) $(QEMU_FLAGS) $(TARGET).elf