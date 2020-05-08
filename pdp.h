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

#define NO_CH_N 0
#define CH_N 1
#define PUTS_1_N 2
#define PUTS_0_N 3

#define NO_CH_Z 0
#define CH_Z 4
#define PUTS_1_Z 8
#define PUTS_0_Z 12

#define NO_CH_C 0
#define CH_C 16
#define PUTS_1_C 32
#define PUTS_0_C 48

typedef struct {
    word mask;
    word opcore;
    char * name;
    void (*do_func) ();
    int params;
    int PSW;
} Command;

Arg get_mr(word w);
int get_NN(word w);
int get_R(word w);

void do_mov();
void do_movb();
void do_add();
void do_halt();
void do_clear();
void do_SOB();
