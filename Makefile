CC=bcc
AS=as86
LD=ld86
CFLAGS = -ansi -c

all: kernel 

kernel.o: kernel.c
	$(CC) $(CFLAGS) -o kernel.o kernel.c

kernel_asm.o: kernel.asm
	$(AS) kernel.asm -o kernel_asm.o

bootload: bootload.asm
	nasm bootload.asm

floppya.img: bootload map.img dir.img loadFile shell testProcess help
	dd if=/dev/zero of=floppya.img bs=512 count=2880
	dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
	dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
	dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
	./loadFile message.txt
	./loadFile tstprg
	./loadFile tstpr2
	./loadFile shell
	./loadFile phello
	./loadFile testProcess
	./loadFile help;

loadFile: loadFile.c
	gcc -o loadFile loadFile.c

shell.o: shell.c
	$(CC) $(CFLAGS) -o shell.o shell.c

help.o: help.c
	$(CC) $(CFLAGS) -o help.o help.c

lib_asm.o: lib.asm
	$(AS) lib.asm -o lib_asm.o

shell: shell.o lib_asm.o
	$(LD) -o shell -d shell.o lib_asm.o

help: help.o lib_asm.o
	$(LD) -o help -d help.o lib_asm.o

testProcess.o: testProcess.c
	$(CC) $(CFLAGS) -o testProcess.o testProcess.c

testProcess: testProcess.o video.o
	$(LD) -o testProcess -d testProcess.o video.o

video.o: video.asm
	$(AS) video.asm -o video.o

kernel: kernel.o kernel_asm.o floppya.img
	$(LD) -o kernel -d kernel.o kernel_asm.o
	dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

clean:
	rm -rf kernel *.o *.out
	rm -rf shell *.o *.out
	rm -rf testProcess *.o *.out
	rm -rf help *.o *.out
	rm -rf video *.o *.out
	rm -rf bootload floppya.img loadFile
