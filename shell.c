#define MAX_FILE_SIZE 13312
char fileBuffer[MAX_FILE_SIZE];

void clearArray(char* a, int size);
int secondFileStart(char* fileNames);
void parseInput(char *terminalBuffer);
int strlen(char *s);
int mod(int a, int b);
int div(int a, int b);

int main() {
  char terminalBuffer[80];
  char shell[8];
  shell[0] = 'S';
  shell[1] = 'H';
  shell[2] = 'E';
  shell[3] = 'L';
  shell[4] = 'L';
  shell[5] = '>';
  shell[6] = ' ';
  shell[7] = '\0';
  enableInterrupts();
  while (1) {
    clearArray(terminalBuffer, 80);
    clearArray(fileBuffer, MAX_FILE_SIZE);
    interrupt(0x21, 0, shell, 0, 0);
    interrupt(0x21, 1, terminalBuffer, 0, 0);
    parseInput(terminalBuffer);
  }
}

int strlen(char *string) {
  int len;
  len = 0;
  while (string[len] != 0) len++;
  return len;
}

void clearArray(char* a, int size) {
  int i;
  i = 0;
  for (i; i < size; i++) {
    a[i] = '\0';
  }
}

int secondFileStart(char *fileNames) {
  int count = 0;
  while (fileNames[count] != ' ' && fileNames[count] != 0x0) {
    count++;
  }
  return count;
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

void parseInput(char *terminalBuffer) {
  int i, j, secondStart, sectors, tail;
  char tempbuf[7], dirTemp[512], typed[80], endline[3];
  char tab[2], sectorCountOutput[3], printSector[8], feed[17];
  feed[0] = 'F';
  feed[1] = 'E';
  feed[2] = 'E';
  feed[3] = 'D';
  feed[4] = ' ';
  feed[5] = 'M';
  feed[6] = 'E';
  feed[7] = ' ';
  feed[8] = 'A';
  feed[9] = ' ';
  feed[10] = 'L';
  feed[11] = 'I';
  feed[12] = 'N';
  feed[13] = 'E';
  feed[14] = '>';
  feed[15] = ' ';
  feed[16] = '\0';
  printSector[0] = 's';
  printSector[1] = 'e';
  printSector[2] = 'c';
  printSector[3] = 't';
  printSector[4] = 'o';
  printSector[5] = 'r';
  printSector[6] = 's';
  printSector[7] = '\0';

  sectors = 0;
  endline[0] = '\r';
  endline[1] = '\n';
  endline[2] = '\0';
  tab[0] = '\t';
  tab[1] = '\0';
  if (terminalBuffer[0] == 't'
      && terminalBuffer[1] == 'y'
      && terminalBuffer[2] == 'p'
      && terminalBuffer[3] == 'e'
      && terminalBuffer[4] == ' '
      && terminalBuffer[7] != '\0') {
    terminalBuffer += 5;
    clearArray(tempbuf, 7);
    for (i = 0; i < 6; i++) {
      if (terminalBuffer[i] == '\0' || terminalBuffer[i] == '\r' ||
          terminalBuffer[i] == '\n' || terminalBuffer[i] == ' ') {
        tempbuf[i] = '\0';
        break;
      } else {
        tempbuf[i] = terminalBuffer[i];
      }
    }
    tempbuf[6] = '\0';
    interrupt(0x21, 3, tempbuf, fileBuffer, 0);
    interrupt(0x21, 0, fileBuffer, 0, 0);
  } else if (terminalBuffer[0] == 'e'
             && terminalBuffer[1] == 'x'
             && terminalBuffer[2] == 'e'
             && terminalBuffer[3] == 'c'
             && terminalBuffer[4] == 'u'
             && terminalBuffer[5] == 't'
             && terminalBuffer[6] == 'e'
             && terminalBuffer[7] == ' '
             && terminalBuffer[10] != '\0') {
    terminalBuffer += 8;
    clearArray(tempbuf, 7);
    for (i = 0; i < 6; i++) {
      if (terminalBuffer[i] == '\0' || terminalBuffer[i] == '\r' ||
          terminalBuffer[i] == '\n' || terminalBuffer[i] == ' ') {
        tempbuf[i] = '\0';
        break;
      } else {
        tempbuf[i] = terminalBuffer[i];
      }
    }
    tempbuf[6] = '\0';
    interrupt(0x21, 4, tempbuf, 0, 0);
  } else if (terminalBuffer[0] == 'e'
             && terminalBuffer[1] == 'x'
             && terminalBuffer[2] == 'e'
             && terminalBuffer[3] == 'c'
             && terminalBuffer[4] == 'f'
             && terminalBuffer[5] == 'o'
             && terminalBuffer[6] == 'r'
             && terminalBuffer[7] == 'e'
             && terminalBuffer[8] == 'g'
             && terminalBuffer[9] == 'r'
             && terminalBuffer[10] == 'o'
             && terminalBuffer[11] == 'u'
             && terminalBuffer[12] == 'n'
             && terminalBuffer[13] == 'd'
             && terminalBuffer[14] == ' '
             && terminalBuffer[17] != '\0') {
    terminalBuffer += 15;
    clearArray(tempbuf, 7);
    for (i = 0; i < 6; i++) {
      if (terminalBuffer[i] == '\0' || terminalBuffer[i] == '\r' ||
          terminalBuffer[i] == '\n' || terminalBuffer[i] == ' ') {
        tempbuf[i] = '\0';
        break;
      } else {
        tempbuf[i] = terminalBuffer[i];
      }
    }
    tempbuf[6] = '\0';
    interrupt(0x21, 4, tempbuf, 1, 0);
  } else if (terminalBuffer[0] == 'd'
             && terminalBuffer[1] == 'e'
             && terminalBuffer[2] == 'l'
             && terminalBuffer[3] == 'e'
             && terminalBuffer[4] == 't'
             && terminalBuffer[5] == 'e'
             && terminalBuffer[6] == ' '
             && terminalBuffer[9] != '\0') {
    terminalBuffer += 7;
    clearArray(tempbuf, 7);
    for (i = 0; i < 6; i++) {
      if (terminalBuffer[i] == '\0' || terminalBuffer[i] == '\r' ||
          terminalBuffer[i] == '\n' || terminalBuffer[i] == ' ') {
        tempbuf[i] = '\0';
        break;
      } else {
        tempbuf[i] = terminalBuffer[i];
      }
    }
    tempbuf[6] = '\0';
    interrupt(0x21, 7, tempbuf, 0, 0);

  } else if (terminalBuffer[0] == 'c'
             && terminalBuffer[1] == 'o'
             && terminalBuffer[2] == 'p'
             && terminalBuffer[3] == 'y'
             && terminalBuffer[4] == ' '
             && terminalBuffer[7] != '\0') {
    terminalBuffer += 5;

    secondStart = secondFileStart(terminalBuffer) + 1;

    clearArray(tempbuf, 7);
    for (i = 0; i < 6; i++) {
      if (i >= secondStart - 1) {
        tempbuf[i] = '\0';
      } else {
        tempbuf[i] = terminalBuffer[i];
      }
    }
    interrupt(0x21, 3, tempbuf, fileBuffer, 0);

    terminalBuffer += secondStart;
    clearArray(tempbuf, 7);
    for (i = 0; i < 6; i++) {
      if (terminalBuffer[i] == '\0' || terminalBuffer[i] == '\r' ||
          terminalBuffer[i] == '\n' || terminalBuffer[i] == ' ') {
        tempbuf[i] = '\0';
        break;
      } else {
        tempbuf[i] = terminalBuffer[i];
      }
    }
    tempbuf[6] = '\0';
    interrupt(0x21, 8, tempbuf, fileBuffer, div(strlen(fileBuffer), 512) + 1);

  } else if (terminalBuffer[0] == 'k'
             && terminalBuffer[1] == 'i'
             && terminalBuffer[2] == 'l'
             && terminalBuffer[3] == 'l'
             && terminalBuffer[4] == ' '
             && terminalBuffer[7] != '\0') {
    terminalBuffer += 5;
    interrupt(0x21, 9, terminalBuffer[0] - '0', 0, 0);
  } else if ((terminalBuffer[0] == 'd'
              && terminalBuffer[1] == 'i'
              && terminalBuffer[2] == 'r'
              && terminalBuffer[3] == '\r') ||
             (terminalBuffer[0] == 'l'
              && terminalBuffer[1] == 's'
              && terminalBuffer[2] == '\r')) {
    interrupt(0x21, 2, dirTemp, 2, 0);
    clearArray(tempbuf, 7);
    clearArray(sectorCountOutput, 3);
    for (i = 0; i < 512; i += 32) {
      if (dirTemp[i] != '\0') {
        for (j = 0; j < 6; j++) {
          tempbuf[j] = dirTemp[i + j];
        }
        for (j = 0; j < 26; j++) {
          if (dirTemp[i + 6 + j] != 0x0) {
            sectors++;
          } else {
            break;
          }
        }
        for (j = 0; j < 2; j++) {
          tail = mod(sectors, 10);
          sectors = div(sectors, 10);
          sectorCountOutput[1 - j] = tail + 48;
        }
        interrupt(0x21, 0, tempbuf, 0, 0);
        interrupt(0x21, 0, tab, 0, 0);
        interrupt(0x21, 0, sectorCountOutput, 0, 0);
        interrupt(0x21, 0, printSector, 0, 0);
        interrupt(0x21, 0, endline, 0, 0);
      }
    }
  } else if (terminalBuffer[0] == 'c'
             && terminalBuffer[1] == 'r'
             && terminalBuffer[2] == 'e'
             && terminalBuffer[3] == 'a'
             && terminalBuffer[4] == 't'
             && terminalBuffer[5] == 'e'
             && terminalBuffer[6] == ' '
             && terminalBuffer[9] != '\0') {
    terminalBuffer += 7;
    i = 0;
    while (1) {
      if (i == MAX_FILE_SIZE) {
        break;
      }
      interrupt(0x21, 0, feed, 0, 0);
      interrupt(0x21, 1, typed, 0, 0);
      if (typed[0] == '\r') {
        break;
      }
      for (j = 0; j < strlen(typed); j++) {
        fileBuffer[i] = typed[j];
        i++;
      }
      interrupt(0x21, 0, fileBuffer, 0, 0);
    }
    clearArray(tempbuf, 7);
    for (i = 0; i < 6; i++) {
      if (terminalBuffer[i] == '\0' || terminalBuffer[i] == '\r' ||
          terminalBuffer[i] == '\n' || terminalBuffer[i] == ' ') {
        tempbuf[i] = '\0';
        break;
      } else {
        tempbuf[i] = terminalBuffer[i];
      }
    }
    tempbuf[6] = '\0';
    interrupt(0x21, 8, tempbuf, fileBuffer, div(strlen(fileBuffer), 512) + 1);
  } else if ((terminalBuffer[0] == 'c'
              && terminalBuffer[1] == 'l'
              && terminalBuffer[2] == 's'
              && terminalBuffer[3] =='\r') ||
             (terminalBuffer[0] == 'c'
              && terminalBuffer[1] == 'l'
              && terminalBuffer[2] == 'e'
              && terminalBuffer[3] == 'a'
              && terminalBuffer[4] == 'r'
              && terminalBuffer[5] =='\r')) {
    interrupt(0x21, 10, 0, 0, 0);
  } else if (terminalBuffer[0] == 'b'
             && terminalBuffer[1] == 'c'
             && terminalBuffer[2] == 'o'
             && terminalBuffer[3] == 'l'
             && terminalBuffer[4] == 'o'
             && terminalBuffer[5] == 'r'
             && terminalBuffer[6] == ' '
             && terminalBuffer[9] != '\0') {
    terminalBuffer += 7;
    clearArray(tempbuf, 7);
    for (i = 0; i < 6; i++) {
      if (terminalBuffer[i] == '\0' || terminalBuffer[i] == '\r' ||
          terminalBuffer[i] == '\n' || terminalBuffer[i] == ' ') {
        tempbuf[i] = '\0';
        break;
      } else {
        tempbuf[i] = terminalBuffer[i];
      }
    }
    tempbuf[6] = '\0';
    interrupt(0x21, 12, stoi(tempbuf), 0, 0);
  } else if (terminalBuffer[0] == 'v'
             && terminalBuffer[1] == 'i'
             && terminalBuffer[2] == 'd'
             && terminalBuffer[3] == 'e'
             && terminalBuffer[4] == 'o'
             && terminalBuffer[5] == ' '
             && terminalBuffer[8] != '\0') {
    terminalBuffer += 6;
    interrupt(0x21, 0, terminalBuffer, 0, 0);
    interrupt(0x21, 13, stoi(terminalBuffer), 0, 0);
  } else if (terminalBuffer[0] == 'p'
             && terminalBuffer[1] == 's'
             && terminalBuffer[2] == '\r') {
    interrupt(0x21, 30, 0, 0, 0);
  } else if (terminalBuffer[0] == 'q'
             && terminalBuffer[1] == 'u'
             && terminalBuffer[2] == 'i'
             && terminalBuffer[3] == 't'
             && terminalBuffer[4] == '\r') {
    interrupt(0x21, 100, 0, 0, 0);
  } else if (terminalBuffer[0] == 'h'
             && terminalBuffer[1] == 'e'
             && terminalBuffer[2] == 'l'
             && terminalBuffer[3] == 'p'
             && terminalBuffer[4] == '\r') {
    interrupt(0x21, 11, 0, 0, 0);
  } else {
    interrupt(0x21, 0, "Bad Command!\r\n\0", 0, 0);
  }
}

int stoi(char *s){
  int i;
  i = 0;
  while (*s >= '0' && *s <= '9')
  {
    i = i * 10 + (*s - '0');
    s++;
  }
  return i;
}