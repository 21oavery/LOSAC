#define LOSAC_TNULL 0
#define LOSAC_EQ 1
#define LOSAC_NEQ 2
#define LOSAC_LESS 3
#define LOSAC_LESSEQ 4

#define LOSAC_CONTAINS 4
#define LOSAC_NCONTAINS 5

#define LOSAC_OPNONE 0
#define LOSAC_ADD 1
#define LOSAC_SUB 2
#define LOSAC_MUL 3
#define LOSAC_DIV 4
#define LOSAC_EXP 5
#define LOSAC_MOD 6

#define LOSAC_CONST 7
#define LOSAC_CONSTD 8
#define LOSAC_IMP 9

#define losac_opHasVarP2(x) (x < 7)
#define losac_opHasVarP3(x) (losac_opHasVarP2(x) && (x != LOSAC_OPNONE))
#define losac_opIsConst(x) (x >= 7)

#define LOSAC_CALC 0
#define LOSAC_FUNC 1
#define LOSAC_FORM 2

struct losac_statement {
    union {
        struct {
            unsigned long p2;
            unsigned long p3;
        } p;
        double d;
    } pd;
    unsigned long p1;
    unsigned char type;
    unsigned char op;
}

struct losac_task {
    struct losac_statement *data;
    unsigned long s;
    unsigned long sMem;
    unsigned long target; // Only used for calculations, sets target
    unsigned char t;
}

// If A = B where both are variables, replace A with B
void removeVarEq(struct losac_task *in) {
    while (true) {
        unsigned long i;
        for (i = 0; i < in->s; i++) if ((in->data[i].t == LOSAC_EQ) && (in->data[i].op == LOSAC_OPNONE)) break;
        if (i == in->s) return;
        unsigned long f = in->data[i].p1;
        unsigned long r = in->data[i].pd.p.p2;
        for (unsigned long j = 0; j < in->s; j++) {
            if (in->data[j].p1 == f) in->data[j].p1 = r;
            if (losac_opHasP2(in->data[j].op) && (in->data[j].pd.p.p2 == f)) in->data[j].pd.p.p2 = r;
            if (losac_opHasP3(in->data[j].op) && (in->data[j].pd.p.p3 == f)) in->data[j].pd.p.p3 = r;
        }
    }
}

// Detects A != A, A < A, A does not contain A
unsigned long detectFault(struct losac_task *in) {
    for (unsigned long i = 0; i < in->s; i++) {
        if ((in->data[i].op == LOSAC_OPNONE) && (in->data[i].p1 == in->data[i].pd.p.p2)) {
            switch (in->data[i].type) {
                case LOSAC_NEQ:
                case LOSAC_LESS:
                case LOSAC_NCONTAINS:
                    return i;
            }
        }
    }
    return in->s;
}

void carryConstant(struct losac_task *in) {
    for (unsigned long i = 0; i < in->s; i++) {
