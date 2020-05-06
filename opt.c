#include "pdp.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

word reg[8];
byte mem[MEMSIZE];



int way = to_mem;

void test_mem () {
    byte b0 = 0x0a;
//пишем байт, читаем байт
    b_write(2, b0);
    byte bres = b_read(2);
    printf("bw/br \t  %hhx = %hhx\n", b0, bres);
    assert(b0 == bres);
//пишем 2 байта подряд, читаем слово
    adr a = 4;
    byte b1 = 0xcb;
    b0 = 0x0c;
    word w = 0xcb0c;
    b_write (a, b0);
    b_write (a+1, b1);
    word wres = w_read(a);
    printf("bw/wr \t %04hx = %02hhx%02hhx \n", wres, b1, b0);
    assert(w == wres);
//пишем слово, читаем слово
    adr  b = 8;
    word w1 = 0xcb0c;
    w_write(b, w1, 1);
    word w2 = w_read(b);
    printf("ww/wr \t %04hx = %04hx \n", w1, w2);
    assert(w1 == w2);
}

void w_write (adr adr, word w, int way) {
	if (way) {
    word b0 = ((byte) w);           //преобразование b0 = w & 0xFF
    word b1 =  ((byte) (w >> 8));   //преобразование (b0 = w >> 8) & 0xFF
    b_write (adr, b0);
    b_write (adr+1, b1);
	}
	else if (!way) {
		reg[adr] = w;
	}
}

word w_read (adr adr) {
	
    return ((word)mem[adr+1]) << 8 | mem[adr]; //первое значение
    // считываем и перемещаем на первый бит (т.е. на 8 "наших" битов) остается ...х00
    // нулевое значение считываем на первый бит
}

void b_write(adr adr, byte b){
    mem[adr] = b;
}

byte b_read(adr adr){
    return mem[adr];
}

void check_reg() {
    for (int i = 0; i < 8; i++ ) {
        printf(" R%d, %06ho ", i, reg[i]);
    }
}

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
			fprintf (stderr, "Mode %o NOT IMPLEMENTED yet!\n", m);
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
