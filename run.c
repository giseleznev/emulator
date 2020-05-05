#include "pdp1.h"
#include <stdio.h>
#include <stdlib.h>

void load_file() {
	FILE *S;
	S = fopen("t.txt", "r");
	int i;
	adr adress;
	word w;
	int n;
	while(1) {
		if (fscanf(S, "%hx", &adress) != EOF ) {
			fscanf(S, "%x", &n);
			for ( i = 0; i < n; i ++) {
				fscanf(S, "%ho", &w);
		//		printf("%06ho \n", w);
				w_write(adress, w, 1);
				adress += 2;
			}
		}
		else break;
	}
}

void mem_dump(adr start, word n) {
	adr adress;
	for(int i = 0; i < 2*n; i +=2 ) {
		word w = w_read(start + i);
		adress = start + i;
		printf("%06ho : %06ho \n", adress, w);
	}
}

void run () {
    pc = 01000; // это слово, поэтому   = ___0 1000
    while(1) {
        word w = w_read(pc);
        printf ("%06o %06o: ", pc, w); // шесть восьмеричных значений   машинное слово: 0.100 .000.0 00.00 0.000
        pc += 2;
        dd = get_mr(w);
        ss = get_mr(w >> 6); 
        for (unsigned int i = 0; i < sizeof(cmd)/sizeof(cmd[0]); i++ ) {
             if ( (w & cmd[i].mask) == cmd[i].opcore) {
                 cmd[i].do_func();
                 printf("\n");
            }
        }
    }
}

int main() {
	load_file();
	run();
	return 0;
}
