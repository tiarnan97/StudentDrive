# Specify the compiler to use
CC=arm-none-eabi-gcc
# Specify the assembler to use
AS=arm-none-eabi-as
# Specify the linker to use
LD=arm-none-eabi-ld

CCFLAGS=-mcpu=cortex-m0 -mthumb -g  
# Tell the linker where to find the libraries -> important: use thumb versions
LIBSPEC=-L "C:\Program Files\arm_sdk\lib\gcc\arm-none-eabi\4.8.3\armv6-m"

# List the object files involved in this project
OBJS=	init.o \
	main.o 

# The default 'target' (output) is main.elf and it depends on the object files being there.
# These object files are linked together to create main.elf
main.elf : $(OBJS)
	$(LD) $(OBJS) $(LIBSPEC) -lgcc -T linker_script.ld --cref -Map main.map -nostartfiles -o main.elf
	objcopy -O ihex main.elf main.hex
	@echo "Assuming all went well..."
	@echo "Program the chip as follows: lpc21isp main.hex /dev/ttyUSB0 9600 12000000"
	@echo "replace ttyUSB0 with the correct device name for your system"	
# The object file main.o depends on main.c.  main.c is compiled to make main.o
main.o: main.c
	$(CC) -c $(CCFLAGS) main.c -o main.o


init.o: init.c
	$(CC) -c $(CCFLAGS) init.c -o init.o

# if someone types in 'make clean' then remove all object files and executables
# associated wit this project
clean: 
	rm $(OBJS) 
	rm main.elf 
