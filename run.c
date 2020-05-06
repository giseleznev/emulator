#include "pdp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Command cmd[] = {
        {0170000, 0000000, "halt", do_halt, has_ss_and_dd},
        {0170000, 0010000, "mov", do_mov, has_ss_and_dd},
        {0170000, 0060000, "add", do_add, has_ss_and_dd},
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
	int n;
	int test = 0;
	while(1) {
		if (fscanf(S, "%hx", &adress) != EOF) {
			test++;
			if (!((0 <= adress)&&(adress <= 64*1024))){ 
					fprintf (stderr, "Invalid data, wrong adress!/n");
					exit(3);
				};
			fscanf(S, "%x", &n);
			for ( i = 0; i < n; i ++) {
				if (fscanf(S, "%hhx", &b) == 0){ 
					fprintf (stderr, "Invalid data, too few bits written!/n");
					exit(3);
				};
				b_write(adress + i, b);
			}
		}
		else {
			if (!test) {
				fprintf (stderr, "Invalid data, nothing was read!/n");
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
        for (unsigned int i = 0; i < sizeof(cmd)/sizeof(cmd[0]); i++ ) {
             if ( (w & cmd[i].mask) == cmd[i].opcore) {
				 if ((cmd[i].params & 1) == has_ss) {
					ss = get_mr(w >> 6);
				 }
				 if ((cmd[i].params & 2) == has_dd) {
					dd = get_mr(w);
				 }
                 cmd[i].do_func();
                 printf("\n");
            }
        }
    }
}

int main(int argc, char **argv) {
	load_file(argv[1]);
	run();
	return 0;
}
