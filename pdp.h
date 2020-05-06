#include <stdio.h>
#include <assert.h>

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
void test_mem();

#define to_register (0)
#define to_mem (1)

#define pc reg[7]

void check_reg();


typedef struct Arg {
    word val;
    word adr;
} Arg;

#define no_params 0
#define has_ss 1
#define has_dd 2
#define has_ss_and_dd 3

typedef struct {
    word mask;
    word opcore;
    char * name;
    void (*do_func) ();
    char params;
} Command;

Arg ss, dd;

Arg get_mr(word w);

void do_mov();
void do_add();
void do_halt();
