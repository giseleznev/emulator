#include <stdio.h>
#include <assert.h>

typedef unsigned char byte; // 8 bit
typedef unsigned short int word; //16 bit
typedef word adr; //16 bit


#define MEMSIZE (64*1024)

void b_write(adr adr, byte b, int way);
byte b_read(adr adr);
void w_write(adr adr, word w, int way);
word w_read(adr adr);
void test_mem();

#define to_register (0)
#define to_mem (1)


void check_reg();


typedef struct Arg {
    word val;
    word adr;
} Arg;

#define NO_PARAMS 0
#define HAS_SS 1
#define HAS_DD 2
#define HAS_NN 4
#define HAS_R 8
#define HAS_B 16
#define HAS_XX 32

typedef struct {
    word mask;
    word opcore;
    char * name;
    void (*do_func) ();
    int params;
} Command;

Arg get_mr(word w);
int get_NN(word w);
int get_R(word w);
int get_XX(word w);

void do_mov();
void do_movb();
void do_add();
void do_halt();
void do_clear();
void do_SOB();
void do_br();
void do_beq();
