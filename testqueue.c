#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "o1_queue.h"

int main() {
    int num = 1000;
    o1_queue *q = queue_new(num);
    // dump(q, "init");

    enqueue(q, 1);
    printf("--------> enqueue peek: %d\n", queue_peek(q));
    enqueue(q, 2);
    printf("--------> enqueue peek: %d\n", queue_peek(q));
    enqueue(q, 3);
    printf("--------> enqueue peek: %d\n", queue_peek(q));
    enqueue(q, 4);
    printf("--------> enqueue peek: %d\n", queue_peek(q));
    enqueue(q, 5);

    printf("--------> dequeue: %d\n", dequeue(q));
    printf("--------> dequeue: %d\n", dequeue(q));
    printf("--------> dequeue: %d\n", dequeue(q));
    printf("--------> dequeue: %d\n", dequeue(q));
    printf("--------> dequeue: %d\n", dequeue(q));


    for (int i = 0; i < num; i++) {
        enqueue(q, i);
    }

    dump(q, "end");

    queue_destroy(q);

    return 0;
}
