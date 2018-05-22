#define LOSAC_EQ 0
#define LOSAC_NEQ 1
#define LOSAC_LESS 2
#define LOSAC_LESSEQ 3

#define LOSAC_CONTAINS 4
#define LOSAC_NCONTAINS 5

#define LOSAC_ADD
#define LOSAC_SUB
#define LOSAC_MUL
#define LOSAC_DIV

struct losac_statement {
    unsigned long p1;
    union {
        unsigned long p2;
        unsigned long p3;
    unsigned char type;
    unsigned char op;
}

/* If A = B, replace A with B
int iterateRemoveEq(struct losac_statement *data, unsigned long preserve, unsigned int length) {
    for 
