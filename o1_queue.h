#ifndef __O1_QUEUE_H__
#define __O1_QUEUE_H__

typedef struct o1_queue_s o1_queue;

o1_queue* queue_new(int size);

void queue_destroy(o1_queue *q);

int queue_peek(o1_queue *q);

void enqueue(o1_queue *q, int v);

int dequeue(o1_queue *q);

void dump(o1_queue *q, const char *opt_str);

#endif /* __O1_QUEUE_H__ */
