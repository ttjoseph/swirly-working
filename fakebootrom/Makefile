# Makefile for fbr - fake boot rom for Swirly
# yeah this makefile sucks
AS = sh4-linux-as
LD = sh4-linux-ld
SRC = main.s
OBJ = $(SRC:.s=.o)
TEXTBASE = 0xa0000000
ASFLAGS = -little
LDFLAGS = -EL --oformat binary -Ttext $(TEXTBASE)

fbr: $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o fbr.bin

clean:	
	rm -f $(OBJ) fbr.bin
	
main.o: main.s
	$(AS) $(ASFLAGS) -o main.o main.s

