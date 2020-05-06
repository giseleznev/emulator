#include <stdlib.h>
#include "pdp.h"

#define to_register (0)
#define to_mem (1)

#define pc reg[7]

void check_reg() {
    for (int i = 0; i < 8; i++ ) {
        printf(" R%d, %06ho ", i, reg[i]);
    }
}
typedef struct Arg {
    word val;
    word adr;
} Arg;


typedef struct {
    word mask;
    word opcore;
    char * name;
    void (*do_func) ();
} Command;

Arg ss, dd;
int way = to_mem;

Arg get_mr(word w) {
	Arg res;
	int r = w & 7;
	int m = (w >> 3) & 7;
	switch (m) {
		case 0: //R0
			res.adr = r;
			res.val = reg[r];
			printf("R%o ", r);
			way = to_register;
			break;
		case 1: // (R3)
			res.adr =reg[r];
			res.val = w_read(res.adr); //b_read
			printf("(R%o) ", r);
			break;
		case 2: // (R3)+ #3
			res.adr =reg[r];
			res.val = w_read(res.adr); //b_read
			
			reg[r] += 2;
			if ( r == 7 ) {
			printf("#%o ", res.val);
		}	
			else {
			printf("(R%o)+ ", r);
		}
		break;
			
		default:
			fprintf (stderr, "Mode %o NOT IMPLEMENTED yet!/n", m);
			exit(1); 
	}
	return res;
}

void do_mov() {
	printf ("mov");
	w_write(dd.adr, ss.val, way);
}

void do_add() {
    printf ("add");
    dd.val = ss.val + dd.val;
    w_write(dd.adr, dd.val, way);
}

void do_halt() {
    printf ("THE END \n");
    	check_reg();
    exit (0);
}

Command cmd[] = {
        {0170000, 0000000, "halt", do_halt},
        {0170000, 0010000, "mov", do_mov},
        {0170000, 0060000, "add", do_add},
};
