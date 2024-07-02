#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "o1_queue.h"

#ifdef DEBUG
void dump(o1_queue *q, const char *opt_str);
#define dump_queue(...) dump(__VA_ARGS__)
#else
#define dump_queue(...)
#endif

typedef struct {
    int top;
    int size;
    int *data;
} stack;

stack* create(int size) {
    stack *s = (stack *)malloc(sizeof(stack));
    s->data = (int *)malloc(sizeof(int) * size);
    s->size = size;
    s->top = -1;
    return s;
}

void destory(stack *s) {
    free(s->data);
    free(s);
}

int full(stack* s) {
    return s->top == s->size - 1;
}

int empty(stack* s) {
    return s->top == -1;
}

void push(stack* s, int v) {
    s->data[++s->top] = v;
}

int pop(stack* s) {
    return s->data[s->top--];
}

int peek(stack* s) {
    return s->data[s->top];
}

#define STACK_NUM 5
enum StackIndex {
    H = 0,
    T,
    Hp,
    Tp,
    Hr,
};
#define STACK_CREATE(size) create(size)
#define STACK_DESTROY(s) destory(s)
#define STACK_PEEK(Q, idx) peek(Q->s[idx])
#define STACK_PUSH(Q, idx, v) push(Q->s[idx], v)
#define STACK_POP(Q, idx) pop(Q->s[idx])
#define STACK_EMPTY(Q, idx) empty(Q->s[idx])
#define HT_LENDIFF(Q) (Q->s[H]->top - Q->s[T]->top)
#define COPY_H(Q) do { \
    Q->h->size = Q->s[H]->size;\
    Q->h->top = Q->s[H]->top;\
    Q->h->data = Q->s[H]->data;\
} while(0)

struct o1_queue_s
{
    stack *s[STACK_NUM];
    int needcopy;
    int copying;
    stack *h;  // H 的浅拷贝
};

o1_queue* queue_new(int size) {
    o1_queue *q = (o1_queue *)malloc(sizeof(o1_queue));
    if (!q) return NULL;
    q->needcopy = 0;
    q->copying = 0;
    for (int i = 0; i < STACK_NUM; i++) {
        q->s[i] = STACK_CREATE(size);
    }
    q->h = STACK_CREATE(0);
    return q;
}

void queue_destroy(o1_queue *q) {
    if (q != NULL) {
        for (int i = 0; i < STACK_NUM; i++) {
            STACK_DESTROY(q->s[i]);
        }
        STACK_DESTROY(q->h);
        free(q);
    }
}

int queue_peek(o1_queue *q) {
    return q->copying ? peek(q->h) : STACK_PEEK(q, H);
}

void quit_copying(o1_queue *q) {
    q->copying = 0;
    stack *temp = q->s[H];
    q->s[H] = q->s[Hp];
    q->s[Hp] = temp;
    q->s[Hp]->top = -1;
    temp = q->s[T];
    q->s[T] = q->s[Tp];
    q->s[Tp] = temp;
    q->s[Tp]->top = -1;
    q->s[Hr]->top = -1;
    q->h->top = -1;
}

void step_forward(o1_queue *q) {
    int step = 1;
    do {
        if (q->copying) {
            if (!STACK_EMPTY(q, H) && !STACK_EMPTY(q, T)) {
                STACK_PUSH(q, Hr, STACK_POP(q, H));
                STACK_PUSH(q, Hp, STACK_POP(q, T));
                q->needcopy++;
            } else if (STACK_EMPTY(q, H) && !STACK_EMPTY(q, T)) {
                STACK_PUSH(q, Hp, STACK_POP(q, T));
            } else if (STACK_EMPTY(q, H) && STACK_EMPTY(q, T)) {
                if (q->needcopy > 1) {
                    STACK_PUSH(q, Hp, STACK_POP(q, Hr));
                    q->needcopy--;
                } else {
                    if (q->needcopy == 1) {
                        STACK_PUSH(q, Hp, STACK_POP(q, Hr));
                        q->needcopy--;
                    }
                    return quit_copying(q);
                }
            }
        }
    } while (step--);
}

void enqueue(o1_queue *q, int v) {
    if (q->copying) {
        STACK_PUSH(q, Tp, v);
        step_forward(q);
    } else {
        STACK_PUSH(q, T, v);
        if (HT_LENDIFF(q) <= 0) {
            q->copying = 1;
            COPY_H(q);
            step_forward(q);
        }
    }
    dump_queue(q, "enqueue");
}

int dequeue(o1_queue *q) {
    int ret;
    if (q->copying) {
        ret = pop(q->h);
        q->needcopy--;
        step_forward(q);
    } else {
        ret = STACK_POP(q, H);
        if (HT_LENDIFF(q) <= 0) {
            q->copying = 1;
            COPY_H(q);
            step_forward(q);
        }
    }
    dump_queue(q, "dequeue");
    return ret;
}

void print_stack(stack *s, const char *name) {
    int num = s->top + 1;
    int buf_size = num * 10 + 1;
    char *str = (char *)malloc(buf_size);
    memset(str, '\0', buf_size);
    int offset = 0;
    for (int i = 0; i < num; i++) {
        offset += sprintf(str + offset, "%d,", s->data[i]);
    }
    printf("----------%s %d : %s\n", name, s->top, str);
    free(str);
}

void dump(o1_queue *q, const char* opt_str) {
    printf("======= queue copying %d, needcopy %d, %s\n", q->copying, q->needcopy, opt_str);
    print_stack(q->s[H], "H");
    print_stack(q->s[T], "T");
    print_stack(q->s[Hp], "Hp");
    print_stack(q->s[Tp], "Tp");
    print_stack(q->s[Hr], "Hr");
    print_stack(q->h, "h");
    printf("\n");
}
