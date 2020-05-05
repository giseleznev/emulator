#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>

word reg[8];
#define pc reg[7];

void do_halt() {
    trace ("The end");
    exit (0);
}
void do_mov () {

}
void do_nothing () {

}
typedef struct {
    word mask;
    word opcore;
    char * name;
    void (do_func) (void);

} Command;
Command cmd[] = {
        {0170000, 0010000, "mov", do_mov()},
        {0170000, 0060000, "add", do_add()},
};

void run () {
    pc = 01000; // это слово, поэтому ____ ____ = ___0 1000
    while(1) {
        word w = w_read(pc);
        trace ("%06o %06o: ", pc, w); // шесть восьмеричных значений   машинное слово: 0.100 .000.0 00.00 0.000
        pc +=2;
        if (w == 0) {
            do_halt();
        }
        else if (( w & 0xF000 ) == 0010000) {            //01SSDD == 0001 | --- --- | --- ---  mask: 1111000000000000 = 0170000 (в 8чной)
            trace("mov ");
            do_mov();
        }
        else if (){
            trace();

        }
    }
}