TARGET = attiny45

COMPILE = avr-gcc -Wall -O2  -I. -mmcu=$(TARGET) # -DDEBUG_LEVEL=2

OBJECTS =  main.o


all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

clean:
	rm -f main.hex main.lst main.obj main.cof main.list main.map main.eep.hex main.bin *.o main.s 

main.bin:	$(OBJECTS)
	$(COMPILE) -o main.bin $(OBJECTS) -Wl,-Map,main.map

main.hex:	main.bin
	rm -f main.hex main.eep.hex
	avr-objcopy -j .text -j .data -O ihex main.bin main.hex

cpp:
	$(COMPILE) -E main.c

flash:
	avrdude -c usbasp -p $(TARGET) -U flash:w:main.hex
