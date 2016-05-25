/*
 Copyright Rose-Hulman Institute of Technology
 by Jeremight Goist (goistjt), Brooke Brown (brownba1), Andrew Ma (maas), 4/20/2016
 Team 4
*/

#include "./processTable.h"
#define MAX_FILE_SIZE 13312

int strlen(char *s);
void printString(char *s);
void readString(char *buf);
int mod(int a, int b);
int div(int a, int b);
void readSector(char *buf, int sector);
void writeSector(char *buf, int sector);
void readFile(int bx, int cx);
void writeFile(char *fileName, char *fileBuffer, int numSectors);
void handleInterrupt21(int ax, int bx, int cx, int dx);
void handleTimerInterrupt(int segment, int sp);
void executeProgram(char *name);
void deleteFile(char *fileName);
void terminate();
void killProcess(int pid);
int findFileIndex(char name[]);
int stringEqual(char string1[], char string2[]);
void clearScreen();
void help();
void changeBColor(int color);
void video(int on);
void itos(int num, char *s);
void checkShell();
void ps();
void quit();

PTable processTable[8];
int currentProc;

int main() {
    char boot[7];
    int i, j;

    boot[0] = 't';
    boot[1] = 'e';
    boot[2] = 's';
    boot[3] = 't';
    boot[4] = 'P';
    boot[5] = 'r';
    boot[6] = '\0';

    currentProc = 0;
    for (i = 0; i < 8; i++) {
        processTable[i].active = 0;
        processTable[i].stackPtr = 0xff00;
        processTable[i].waiting = -1;
        processTable[i].name[0] = '\0';
        processTable[i].name[1] = '\0';
        processTable[i].name[2] = '\0';
        processTable[i].name[3] = '\0';
        processTable[i].name[4] = '\0';
        processTable[i].name[5] = '\0';
        processTable[i].name[6] = '\0';
    }

    makeInterrupt21();
    makeTimerInterrupt();
    interrupt(0x21, 13, 1, 0, 0);
    executeProgram(boot, 0);
    while (1);
    return 0;
}

void help() {
    char help[5];
    help[0] = 'h';
    help[1] = 'e';
    help[2] = 'l';
    help[3] = 'p';
    help[4] = '\0';
    executeProgram(help, 0); 
}

void itos(int num, char *s) {
    int i, t;
    i = 0;
    while (num != 0) {
        s[i++] = mod(num, 10) + '0';
        num = div(num, 10);
    }
    s[i] = '\0';
    for (t = 0; t < i / 2; t++) {
        s[t] ^= s[i - t - 1];
        s[i - t - 1] ^= s[t];
        s[t] ^= s[i - t - 1];
    }
    if (num == 0) {
        s[0] = '0';
        s[1] = '\0';
    }
}

void clearScreen() {
    int i;
    char endline[3];
    endline[0] = '\r';
    endline[1] = '\n';
    endline[2] = '\0';
    for (i = 0; i < 100; i++) {
        printString(endline);
    }
}

void changeBColor(int color) {
    interrupt(0x10, 0xb * 256 + 'q', color, 0, 0);
}

void video(int on) {
    if (on){
        interrupt(0x10, 0x13, 0, 0, 0);
    } else {
        interrupt(0x10, 0x3, 0, 0, 0);
    }
}

void terminate() {
    int i;
    setKernelDataSegment();
    processTable[currentProc].active = 0;
    restoreDataSegment();
    for (i = 0; i < 8; i++) {
        setKernelDataSegment();
        if (processTable[i].waiting == currentProc) {
            processTable[i].waiting = -1;
            processTable[i].active = 1;
            break;
        } else {
            restoreDataSegment();
        }
    }
    setKernelDataSegment();

    processTable[currentProc].waiting = -1;
    restoreDataSegment();
    while (1);
}

int strlen(char *string) {
    int len;
    len = 0;
    while (string[len] != 0) len++;
    return len;
}

void printString(char *s) {
    while (*s != 0x0) {
        interrupt(0x10, 0xe * 256 + *s, 3, 0, 0);
        s += 1;
    }
}

void readString(char *buf) {
    int index;
    char returned;
    char word[2];
    word[1] = '\0';
    index = 0;
    while (index < 77) {
        returned = interrupt(0x16, 0, 0, 0, 0);
        word[0] = returned;
        if (returned == 0x8) {
            if (index > 0) {
                index--;
                printString(word);
                word[0] = ' ';
                printString(word);
                word[0] = returned;
                printString(word);
            } else {
                index = 0;
            }
        } else if (returned == 0xd) {
            break;
        } else {
            buf[index] = returned;
            printString(word);
            index++;
        }
    }
    word[0] = '\r';
    printString(word);
    word[0] = '\n';
    printString(word);
    buf[index] = '\r';
    buf[index + 1] = 0xa;
    buf[index + 2] = 0x0;
}

int mod(int a, int b) {
    while (a >= b) {
        a = a - b;
    }
    return a;
}

int div(int a, int b) {
    int quotient = 0;
    while ((quotient  + 1)* b  <= a) {
        quotient  = quotient  + 1;
    }
    return quotient;
}

void readSector(char *buf, int sector) {
    interrupt(0x13, 2 * 256 + 1, buf, div(sector, 36)
              * 256 + (mod(sector, 18) + 1), mod(div(sector, 18), 2) * 256 + 0);
}

void writeSector(char *buf, int sector) {
    interrupt(0x13, 3 * 256 + 1, buf, div(sector, 36)
              * 256 + (mod(sector, 18) + 1), mod(div(sector, 18), 2) * 256 + 0);
}

int findFileIndex(char *name) {
    char sectorBuffer[512];
    char fileName[7];
    int i, j;
    readSector(sectorBuffer, 2);
    for (i = 0; i < 16; i++) {
        /* copy name*/
        for (j = 0; j < 6; j++) {
            fileName[j] = sectorBuffer[i * 32 + j];
        }
        fileName[6] = '\0';

        /* check if it is the right sector*/
        if (stringEqual(fileName, name)) {
            return i;
        }
    }
    return -1;
}

int stringEqual(char *s1, char *s2) {
    int i;
    if (strlen(s1) != strlen(s2)) {
        return 0;
    }
    for (i = 0; i < strlen(s1); i++) {
        if (s1[i] != s2[i]) {
            return 0;
        }
    }
    return 1;
}

void deleteFile(char *fileName) {
    char map[512];
    char directory[512];
    int i, j, k;
    readSector(map, 1);
    readSector(directory, 2);

    /* Iterate through directory entries */
    for (i = 0; i < 512; i += 32) {
        j = 0;
        /* Make sure first 6 chars of filename match directory entry name */
        for (j; j < 6; j++) {
            if (fileName[j] != directory[i + j]) {
                break;
            } else if (j != 0 && fileName[j] == '\0' && directory[i + j] == '\0') {
                j = 6;
                break;
            }
        }
        /* Found the file */
        if (j == 6) {
            directory[i] = 0x0;
            /* Iterate thru directory pointers and mark 0s in map */
            for (k = 0; k < 26; k++) {
                if (directory[i + j + k] == 0x0) {
                    break;
                }
                map[directory[i + j + k]] = 0x0;
            }
            writeSector(map, 1);
            writeSector(directory, 2);
            return;
        }
    }
}

void readFile(char *bx, char *cx) {
    int i, j, k;
    char dirBuf[512];

    readSector(dirBuf, 2);

    for (i = 0; i < 512; i += 32) {
        j = 0;
        for (j = 0; j < 6; j++) {
            if (bx[j] != dirBuf[i + j]) {
                break;
            } else if (bx[j] == '\0' && dirBuf[i + j] == '\0') {
                j = 6;
                break;
            }
        }
        if (j == 6) {
            for (k = 0; k < 26; k++) {
                if (dirBuf[i + j + k] == 0x0) {
                    return;
                }
                readSector(cx, dirBuf[i + j + k]);
                cx += 512;
            }
            return;
        }
    }
}

void writeFile(char *name, char *buffer, int numberOfSectors) {
    char dir[512], writeBuffer[512], map[512];
    int mappedSectors[26];
    int i, j, k;
    readSector(map, 1);
    readSector(dir, 2);
    /* Find open sectors */
    i = 0;
    j = 0;
    while (i < 512) {
        if (map[i] == 0) {
            mappedSectors[j] = i;
            j++;
        }
        /* Found needed number */
        if (j == numberOfSectors) {
            break;
        }
        i++;
    }
    /* Return if not enough sectors available */
    if (j < numberOfSectors) {
        return;
    }
    /* Write the filename to the directory */
    i = 0;
    while (i < 16) {
        /* found an open directory entry */
        if (dir[i * 32] == 0) {
            for (j = 0; j < 6; j++) {
                /* if name is less than 6 characters long, write 0's in rest of space */
                if (name[j] == '\0') {
                    for (k = j; k < 6; k++) {
                        dir[i * 32 + j] = 0;
                    }
                    break;
                }
                dir[i * 32 + j] = name[j];
            }
            break;
        }
        i++;
    }
    /* Write sectors past the name*/
    for (j = 0; j < numberOfSectors; j++) {
        dir[i * 32 + 6 + j] = mappedSectors[j];
    }
    /* Write to the map */
    for (i = 0; i < numberOfSectors; i++) {
        map[mappedSectors[i]] = 0xFF;
        for (j = 0; j < 512; j++) {
            writeBuffer[j] = buffer[i * 512 + j];
        }
        interrupt(0x21, 6, writeBuffer, mappedSectors[i], 0);
    }
    /* Writeback the map and directory */
    writeSector(map, 1);
    writeSector(dir, 2);
}

void executeProgram(char *name, int foreGround) {
    char prog[MAX_FILE_SIZE];
    int i, k, segment, loc, valid;
    char one,two,three,four,five,six;
    one = name[0];
    two = name[1];
    three = name[2];
    four = name[3];
    five = name[4];
    six = name[5];
    loc = 0;

    valid = findFileIndex(name);
    if (valid == -1) {
        /* not a valid program. */
        return;
    }

    /* Find free segment*/
    for (i = 0; i < 8; i++) {
        setKernelDataSegment();
        if (!processTable[i].active) {
            processTable[i].stackPtr = 0xff00;
            restoreDataSegment();
            loc = i;
            segment = (i + 2) * 0x1000;
            break;
        } else {
            restoreDataSegment();
        }
    }

    if (i == 8) {
        return;
    }

    readFile(name, prog);
    for (i = 0; i < 13312; i++) {
        putInMemory(segment, i, prog[i]);
    }
    if (foreGround) {
        setKernelDataSegment();
        processTable[currentProc].waiting = loc;
        restoreDataSegment();
    }
    
    initializeProgram(segment);
    setKernelDataSegment();
    processTable[loc].name[0] = one;
    processTable[loc].name[1] = two;
    processTable[loc].name[2] = three;
    processTable[loc].name[3] = four;
    processTable[loc].name[4] = five;
    processTable[loc].name[5] = six;
    processTable[loc].name[6] = '\0';
    processTable[loc].active = 1;
    restoreDataSegment();
}

void checkShell(){
    char shell[6];
    int i;
    shell[0] = 's';
    shell[1] = 'h';
    shell[2] = 'e';
    shell[3] = 'l';
    shell[4] = 'l';
    shell[5] = '\0';

    for (i = 0; i < 8; i++) {
        setKernelDataSegment();
        if(processTable[i].name[0] == 's'
                && processTable[i].name[1] == 'h'
                && processTable[i].name[2] == 'e'
                && processTable[i].name[3] == 'l'
                && processTable[i].name[4] == 'l'
                && processTable[i].name[5] == '\0') {
            restoreDataSegment();
            break;
        } else {
            restoreDataSegment();
        }
    }
    if (i == 8) {
        executeProgram(shell, 0);
    }
}

void ps() {
    int i;
    char spacedashspace[4], endline[3], p[2];
    spacedashspace[0] = ' ';
    spacedashspace[1] = '-';
    spacedashspace[2] = ' ';
    spacedashspace[3] = '\0';
    endline[0] = '\r';
    endline[1] = '\n';
    endline[2] = '\0';
    p[1] = '\0';

    for(i = 0; i < 8; i++) {
        setKernelDataSegment();
        if (processTable[i].active) {
            restoreDataSegment();
            p[0] = i + '0';
            printString(p);
            printString(spacedashspace);
            setKernelDataSegment();
            printString(processTable[i].name);
            restoreDataSegment();
            printString(endline);
        } else {
            restoreDataSegment();
        }
    }
}

void quit() {
    char *s;

    writeOut (0xB004, 0x2000);
    for (s = "Shutdown"; *s; ++s) {
      writeOut (0x8900, *s);
    }

}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
    switch (ax) {
    case 0:
        printString(bx);
        break;
    case 1:
        readString(bx);
        break;
    case 2:
        readSector(bx, cx);
        break;
    case 3:
        readFile(bx, cx);
        break;
    case 4:
        executeProgram(bx, cx);
        break;
    case 5:
        terminate();
        break;
    case 6:
        writeSector(bx, cx);
        break;
    case 7:
        deleteFile(bx);
        break;
    case 8:
        writeFile(bx, cx, dx);
        break;
    case 9:
        killProcess(bx);
        break;
    case 10:
        clearScreen();
        break;
    case 11:
        help();
    case 12:
        changeBColor(bx);
        break;
    case 13:
        video(bx);
        break;
    case 14:
        checkShell();
        break;
    case 30:
        ps();
        break;
    case 100:
        quit();
        break;
    default:
        /*Error message*/
        break;
    }
}

void handleTimerInterrupt(int segment, int sp) {
    int index, i, tempsp;
    int targetSegment;
    currentProc = div(segment,0x1000) - 2;
    processTable[currentProc].stackPtr = sp;
    for (i = 0; i < 8; i++) {
        if (processTable[mod(currentProc + 1 + i, 8)].active 
                && processTable[mod(currentProc + 1 + i, 8)].waiting == -1) {
            currentProc = mod(currentProc + 1 + i, 8);
            tempsp = processTable[currentProc].stackPtr;
            targetSegment = (currentProc + 2) * 0x1000;
            returnFromTimer(targetSegment,  tempsp);
            return;
        }
    }
    returnFromTimer(segment, sp);
}

void killProcess(int pid) {
    int i;
    setKernelDataSegment();
    processTable[pid].active = 0;
    restoreDataSegment();
    for (i = 0; i < 8; i++) {
        setKernelDataSegment();
        if (processTable[i].waiting == currentProc) {
            processTable[i].waiting = -1;
            processTable[i].active = 1;
            restoreDataSegment();
            break;
        } else {
            restoreDataSegment();
        }
    }
}
