#include "pdp.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#define pc reg[7]

word reg[8];
byte mem[MEMSIZE];

FILE* print_file;

int way = to_mem;

Arg ss, dd;

int XX;
int NN, R;
int B = 0;

char flag_N = 0;
char flag_Z = 0;
char flag_C = 0;

void test_mem () {
    byte b0 = 0x0a;
//пишем байт, читаем байт
    b_write(2, b0, to_mem);
    byte bres = b_read(2);
    printf("bw/br \t  %hhx = %hhx\n", b0, bres);
    assert(b0 == bres);
//пишем 2 байта подряд, читаем слово
    adr a = 4;
    byte b1 = 0xcb;
    b0 = 0x0c;
    word w = 0xcb0c;
    b_write (a, b0, to_mem);
    b_write (a+1, b1, to_mem);
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
    b_write (adr, b0, to_mem);
    b_write (adr+1, b1, to_mem);
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

void b_write(adr adr, byte b, int way){
    if (way) {
		mem[adr] = b;	
		if (adr == odata) {
			fprintf(print_file, "%c", b);
		}
	}
	if (!way) {
		if ((b >> 7)) {
			reg[adr] = (0177400|b);
		}
		if (!(b >> 7)) {
			reg[adr] = (0000377&b);
		}
	}
}

byte b_read(adr adr){
	if (way) {
		return mem[adr];
	}
	else {
		return (0377 & reg[adr]);
	}
}

void check_reg() {
    for (int i = 0; i < 8; i++ ) {
        printf(" R%d, %06ho ", i, reg[i]);
    }
}
void check_flag() {
	printf("N %d Z %d V  C%d ", flag_N, flag_Z, flag_C);
}


int get_NN(word w){
	NN = w & 077;
	return NN;
}

int get_R(word w){
	R = (w >> 6) & 07;
	printf("R%o ", R);
	return R;
}

Arg get_mr(word w) {
	Arg res;
	int r = w & 7;
	int m = (w >> 3) & 7;
	way = to_mem;
	switch (m) {
		case 0: //R0
			res.adr = r;
			res.val = reg[r];
			printf("R%o ", r);
			way = to_register;
			break;
		case 1: // (R3)
			res.adr =reg[r];
			if (!B){
				res.val = w_read(res.adr);
			}
			if (B){
				res.val = b_read(res.adr);
			} //b_read
			printf("(R%o) ", r);
			break;
		case 2: // (R3)+ #3
			res.adr = reg[r];
			if (!B){
				res.val = w_read(res.adr);
				reg[r] += 2;		
			}
			else {
			res.val = b_read(res.adr);
			if ((r != 6) && (r != 7)){
				reg[r] += 1;
			}
			else {
				reg[r] += 2;	
			}
			
			} //b_read
			if ( r == 7 ) {
				printf("#%06o ", res.val);
			}	
			else {
				printf("(R%o)+ ", r);
			}
			break;
		case 3: //@(Rn)+
			res.adr = reg[r];
			res.adr = w_read(res.adr);
			res.val = w_read(res.adr);
			reg[r] += 2;
			if ( r == 7 ) {
			printf("@#%o ", res.adr);
			}	
			else {
			printf("@(R%o)+ ", r);
			}
			break;
		case 4:
			if (!B){
				reg[r] -= 2;
				res.adr = reg[r];
				res.val = w_read(res.adr);		
			}
			else {
			if ((r != 6) && (r != 7)){
				reg[r] -= 1;
			}
			else {
				reg[r] -= 2;	
			}
			res.adr = reg[r];
			res.val = b_read(res.adr);
			} 
			printf("-(R%o) ", r);
			break;	
		default:
			fprintf (stderr, "Mode %o NOT IMPLEMENTED yet!\n", m);
			exit(1); 
	}
	return res;
}
void get_XX(word w) {
	if ((w >> 7) & 01) {
		XX = -(0400 - (w & 0377));
	}
	else {
		XX = (w & 0377);
	}
	if (XX){
		printf("%06o", pc + XX*2);
	}
	else {
		printf("%06o", pc - XX*2);
	}
}
void get_flag_N(word w) {
	flag_N = (w >> 15);
}
void get_flag_N_b(byte w) {
	flag_N = (w >> 7);
}
void get_flag_Z(word w) {
	if (w == 0) {
		flag_Z = 1;
	}
	else {
		flag_Z = 0;
	}
}
void get_flag_Z_b(byte w) {
	get_flag_Z(w);
}
void get_flag_C(word w1, word w2) {
	int W1[17];
	int W2[17];
	for (int i = 0; i < 16; i++){
		W1[i] = w1 >> i;
		W2[i] = w2 >> i;
	}
	for (int i = 0; i< 16; i++){
		if ((W1[i] + W2[i]) >=2 ){
			W1[i+1] ++;
		}
	}
	flag_N = (W1[16]);
}

void get_flag_C_b(byte b1, byte b2) {
	int B1[9];
	int B2[9];
	for (int i = 0; i< 8; i++){
		B1[i] = b1 >> i;
		B2[i] = b2 >> i;
	}
	
	for (int i = 0; i< 8; i++){
		if ((B1[i] + B2[i]) >=2 ){
			B1[i+1] ++;
		}
	}
	flag_N = (B1[9]);
}

void do_mov() {
	w_write(dd.adr, ss.val, way);
	
	get_flag_N(ss.val);
	get_flag_Z(ss.val);
		
}

void do_movb() {
	b_write(dd.adr, ss.val, way);
	get_flag_N(ss.val);
	get_flag_Z(ss.val);
}

void do_add() {
	get_flag_C(dd.val, ss.val);	
    dd.val = ss.val + dd.val;
    w_write(dd.adr, dd.val, way);
	get_flag_N(dd.val);
	get_flag_Z(dd.val);
}

void do_halt() {
    	check_reg();
    exit (0);
}
void do_clear() {
	w_write(dd.adr, 0, way);
	flag_C = 0;
	flag_N = 0;
	flag_Z = 1;
}
	
void do_SOB() {
	if ((--reg[R]) != 0) {	
		pc = pc - NN*2;
	}
	printf (" %06o", pc);
}
void do_br() {
	if (XX){
		pc = pc + XX*2;
	}
	else {
		pc = pc - XX*2;
	}
}
void do_beq(){
	if (flag_Z) {
		do_br();
	}
}
void do_bpl(){
	if (!flag_N) {
		do_br();
	}
}
void do_tstb(){
	get_flag_N_b(dd.val);
	get_flag_Z_b(dd.val);
	flag_C = 0;
}
void do_ash(){
	if (!(dd.val >> 15)) {
		reg[R] = reg[R] << dd.val;
		get_flag_C(R, 0);
	}
	else {
		reg[R] = reg[R] >> (0200000-dd.val);
		flag_C = 0;
	}
	get_flag_N(dd.val);
	get_flag_Z(dd.val);
}
void do_bic() {
	reg[dd.adr] = reg[dd.adr] & (~ss.val);
	get_flag_Z(dd.val);
	get_flag_N(dd.val);
}
