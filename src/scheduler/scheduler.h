#ifndef _SCHEDULER_HEADER_FILE
#define _SCHEDULER_HEADER_FILE

#include <stdbool.h>

#include "../types.h"
#include "queue.h"

void schedule(Input input);
void scheduler_init(Queue ready_queues[], Input input);
bool scheduler_is_finished(Queue ready_queues[]);
void scheduler_print_ready_queues(Queue ready_queues[]);
ProcessId scheduler_dequeue_ready_queues(Queue ready_queues[]);

#endif
