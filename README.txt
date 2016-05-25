Andrew Ma, Brooke Brown, Jeremiah Goist - Team04

Tools:

Bochs 2.4.6 - An x86 processor simulator (ver 2.3.5 also works).
bochs-sdl - an SDL (Simple DirectMedia Layer) display library plugin for Bochs
bcc (Bruce Evan's C Compiler) - A 16-bit C compiler
as86, ld86 - A 16 bit assembler and linker that typically comes with bcc
gcc - The standard 32-bit GNU C compiler
nasm - The Netwide Assembler
hexedit - A utility that allows you to edit a file in hexadecimal byte-by-byte
dd - A standard low-level copying utility. This generally comes with UNIX.
Emacs, vi, pico, or nano - A simple text-based editor

To compile the code: type 'make' into the terminal
To run the OS: run bochs with 'bochs -f opsys.bxrc'

Once the OS is running:

To view a list of all files in the directory and the number of sectors each occupies:
    type 'dir' or 'ls'

To view the content of a file:
    type 'type <filename>' using one of the files loaded into the OS 

To create a new file:
    type 'create <filename>' with a filename that does not already exist
    enter all desired lines of text followed by a new line

To copy a file into a new file:
    type 'copy <filename 1> <filename 2>' 
        filename 1 is the existing file name
        filename 2 is the name to create the copied file with

To delete an existing file:
    type 'delete <filename>' where filename is a file in the directory

To execute a program:
    type 'execute <program name>' using one of the programs loaded into the OS

To execute a program and give it keyboard control:
    type 'execforeground <program name>' using one of the programs loaded into the OS

To see the termination of a currently running program:
    run the program using execute or execforeground
    terminate will be called automatically when the program finishes


To terminate a currently running process before execution finishes:
    run the program using execute
    manually terminate the program using 'kill <process id>'
        get the process id using ps (incremental)

------------------------------------------------------------------------------

To view all active processes and their associated process id:
    type 'ps'

To turn graphics on and off:
    type 'video 1' to turn graphics on 
        makes foreground color light blue
    type 'video 0' to turn graphics off
        makes foreground standard white

To change the background color:
    type 'bcolor <number>'
        any number can be used, but there are only 16 different colors
        0 is a black background

To clear the screen:
    type 'clear' or 'cls'

To quit/close bochs:
    type 'quit'

Other features:
    'windows' logo displayed on startup
    'shell' is 'unkillable'. Once you kill shell, we give you the option to run one more program before shell dies so that you can rerun shell incase you accidnetally kill it.

