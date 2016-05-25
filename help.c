int main(){
    interrupt(0x21, 0, "type [filename]               Displays the text of a file.\r\n\0", 0, 0);
    interrupt(0x21, 0, "create [filename]             Create a text file.\r\n\0", 0, 0);
    interrupt(0x21, 0, "copy [filename1] [filename2]  Copy the contents of file1 to file2.\r\n\0", 0, 0);
    interrupt(0x21, 0, "delete [filename]             Deletes the file.\r\n\0", 0, 0);
    interrupt(0x21, 0, "execute [filename]            Executes the file.\r\n\0", 0, 0);
    interrupt(0x21, 0, "execforeground [filename]     Executes the file in the foreground.\r\n\0", 0, 0);
    interrupt(0x21, 0, "kill [0-7]                    Kills a process with the given pid\r\n\0", 0, 0);
    interrupt(0x21, 0, "ps                            Lists all active processes.\r\n\0", 0, 0);
    interrupt(0x21, 0, "ls/dir                        Lists all files.\r\n\0", 0, 0);
    interrupt(0x21, 0, "clear/cls                     Clears the terminal.\r\n\0", 0, 0);
    interrupt(0x21, 0, "bcolor [0-15]                 Changes the background color \r\n\0", 0, 0);
    interrupt(0x21, 0, "video [0-1]                   Turns graphics mode on/off.\r\n\0", 0, 0);
    interrupt(0x21, 0, "help                          Lists all avaiable commands.\r\n\0", 0, 0);
    interrupt(0x21, 0, "quit                          Shutdown and close Bochs.\r\n\0", 0, 0);
    interrupt(0x21, 5, 0, 0, 0);
}