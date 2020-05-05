#include <stdio.h>
#include <assert.h>
#define re (0)
#define me (1)

typedef unsigned char byte; // 8 bit
typedef unsigned short int word; //16 bit
typedef word adr; //16 bit

word reg[8];


#define MEMSIZE (64*1024)

byte mem[MEMSIZE];
void b_write(adr adr, byte b);
byte b_read(adr adr);
void w_write(adr adr, word w, int way);
word w_read(adr adr);

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
