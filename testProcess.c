/*
 Draws windows logo
*/
int main(){
    int i, j, k, x, y, z;
    char shell[6], boot[7];
    shell[0] = 's';
    shell[1] = 'h';
    shell[2] = 'e';
    shell[3] = 'l';
    shell[4] = 'l';
    shell[5] = '\0';

    enableInterrupts();
    

    /*clear screen */
    for (i = 0; i < 200; i ++){
        for (j = 0; j < 600; j ++) {
            putInMemory(0xA000, 0x0000 + i*320 + j, 0x0);
        }
    }

    /* Top left red square */
    for(i = 10; i < 95; i++) {
        for(j = 70; j  < 155; j++) {
            putInMemory(0xA000,  + i*320 + j, 0x4);
        }
    }

    /* Top right green*/
    for(i = 10; i < 95; i++) {
        for(j = 165; j  < 250; j++) {
            putInMemory(0xA000,  + i*320 + j, 0x2);
        }
    }

    /* Bot left blue */
    for(i = 105; i < 190; i++) {
        for(j = 70; j  < 155; j++) {
            putInMemory(0xA000,  + i*320 + j, 0x1);
        }
    }

    /* Bot right yellow */
    for(i = 105; i < 190; i++) {
        for(j = 165; j  < 250; j++) {
            putInMemory(0xA000,  + i*320 + j, 0xe);
        }
    }

    for (k = 0; k < 200; k++){
        for(y = 0; y < 200; y++){
            for(z = 0; z < 100; z++){
                    x = 1 + 1;
            }
        }
    }

    /*clear screen */
    for (i = 0; i < 200; i ++){
        for (j = 0; j < 600; j ++) {
            putInMemory(0xA000, 0x0000 + i*320 + j, 0x0);
        }
    }
    interrupt(0x21, 13, 0, 0, 0);
    interrupt(0x21, 14, 0, 0, 0);

    interrupt(0x21, 5, 0, 0, 0);
}