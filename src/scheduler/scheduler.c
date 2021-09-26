#include "scheduler.h"

void schedule(Input input)
{
    const int NUMBER_OF_READY_QUEUE = 4;
    Queue ready_queues[NUMBER_OF_READY_QUEUE];
    ready_queues[0] = create_queue();
    ready_queues[1] = create_queue();
    ready_queues[2] = create_queue();
    ready_queues[3] = create_queue();

    scheduler_init(ready_queues, input);
    
    print_ready_queues(ready_queues);

    while (!scheduler_is_finished(ready_queues)) {
        // TODO: ready queue 에서 프로세스 하나 뽑고
        // cpu 작업하고
        // 보내고
        // io 작업하고
        // ready queue 로 다시 넣으면 될 듯
    }
}

void scheduler_init(Queue ready_queues[], Input input)
{
    int index_of_process;
    for (index_of_process = 0; index_of_process < input.number_of_process; index_of_process++)
    {
        ProcessInput process_input = input.process_inputs[index_of_process];
        enqueue(&ready_queues[process_input.init_queue], process_input.id);
    }
}

bool scheduler_is_finished(Queue ready_queues[])
{
    return ready_queues[0].length == 0
        && ready_queues[1].length == 0
        && ready_queues[2].length == 0
        && ready_queues[3].length == 0; 
}

void print_ready_queues(Queue ready_queues[])
{
    print_queue(&ready_queues[0]);
    print_queue(&ready_queues[1]);
    print_queue(&ready_queues[2]);
    print_queue(&ready_queues[3]);
}
