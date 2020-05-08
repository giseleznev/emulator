#include "pdp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define pc reg[7]

extern word reg[8];
extern byte mem[MEMSIZE];
extern Arg ss, dd;
extern int NN, R, B;

Command cmd[] = {
        {0170000, 0010000, "mov", do_mov, (HAS_SS|HAS_DD)},
        {0170000, 0110000, "movb", do_movb, (HAS_B|HAS_SS|HAS_DD)},
        {0170000, 0060000, "add", do_add, (HAS_SS|HAS_DD)},
        {0177000, 0077000, "SOB", do_SOB, (HAS_R|HAS_NN)},
        {0177700, 0005000, "clear", do_clear, (HAS_DD)},
        {0170000, 0000000, "halt", do_halt, (NO_PARAMS)},
};

void load_file (char* path) {
    FILE * S  = fopen(path, "r");   
    if (S == NULL) {
        perror(path);
        exit(2);
    }
	int i;
	adr adress;
	byte b;
	int x,n;
	int test = 0;
	while(1) {
		if ((x = fscanf(S, "%hx", &adress) != EOF) && (x != 0)) {
			test++;
			if (!((0 <= adress)&&(adress <= MEMSIZE))){ 
					fprintf (stderr, "Invalid data, wrong adress!\n");
					exit(3);
				};
			fscanf(S, "%x", &n);
			for ( i = 0; i < n; i ++) {
				if (fscanf(S, "%hhx", &b) == 0){ 
					fprintf (stderr, "Invalid data, too few bits written!\n");
					exit(3);
				};
				b_write(adress + i, b, to_mem);
			}
		}
		else {
			if (!test) {
				fprintf (stderr, "Invalid data, nothing was read!\n");
				exit(3);
			}
			break;
		}
	}
}


void mem_dump(adr start, word n) {
	adr adress;
	for(int i = 0; i < n; i += 2) {
		word w = w_read(start + i);
		adress = start + i;
		printf("%06o : %06o \n", adress, w);
	}
}

void run () {
    pc = 01000; // это слово, поэтому   = ___0 1000
    while(1) {
        word w = w_read(pc);
        printf ("%06o %06o: ", pc, w); // шесть восьмеричных значений   машинное слово: 0.100 .000.0 00.00 0.000
        pc += 2;
        check_reg();
        for (unsigned int i = 0; i < sizeof(cmd)/sizeof(cmd[0]); i++ ) {
             if ( (w & cmd[i].mask) == cmd[i].opcore) {
				 if ((cmd[i].params) & (HAS_B)) {
					B = 1;
				 }
				 if ((cmd[i].params) & (HAS_SS)) {
					ss = get_mr(w >> 6);	
				 }
				 if ((cmd[i].params) & (HAS_DD) ){
					dd = get_mr(w);
				 }
				 if ((cmd[i].params) & (HAS_R) ){
					R = get_R(w);
				 }
				 if ((cmd[i].params) & (HAS_NN) ){
					NN = get_NN(w);
				 }
                 cmd[i].do_func();
                 printf("\n");
                 break;
            }
        }
    }
}

int main(int argc, char **argv) {
	if(argv[1] == NULL){
		printf("Invalid command! \nput the file path after programm name\nwrite: ./programme path to a file \n");
		return 1;
	}
	load_file(argv[1]);
	run();
	return 0;
}
