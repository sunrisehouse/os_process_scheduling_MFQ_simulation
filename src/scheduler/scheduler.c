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

    scheduler_print_ready_queues(ready_queues);

    while (!scheduler_is_finished(ready_queues)) {
        // TODO
        // ready queue 에서 프로세스 하나 뽑고
        ProcessId pid = scheduler_dequeue_ready_queues(ready_queues);
        // cpu 작업하고

        // io 보내고
        
        // ready queue 로 다시 넣으면 될 듯
        

        printf("*****one cycle result*********\n");
        scheduler_print_ready_queues(ready_queues);
    }
}

void scheduler_init(Queue ready_queues[], Input input)
{
    ProcessInput ordered_process_inputs[input.number_of_process];

    int i;
    for (i = 0; i < input.number_of_process; i++)
    {
        ordered_process_inputs[i] = input.process_inputs[i];
    }

    for (i = 0; i < input.number_of_process; i++)
    {
        int j;
        for (j = 0; j < input.number_of_process - i - 1; j++)
        {
            if (ordered_process_inputs[i].arrival_time > ordered_process_inputs[i + 1].arrival_time)
            {
                ProcessInput temp = ordered_process_inputs[i + 1];
                ordered_process_inputs[i + 1] = ordered_process_inputs[i];
                ordered_process_inputs[i] = temp;
            }
        }
    }

    int index_of_process;
    for (index_of_process = 0; index_of_process < input.number_of_process; index_of_process++)
    {   
        ProcessId pid = ordered_process_inputs[index_of_process].id;
        InitQueue init_queue = ordered_process_inputs[index_of_process].init_queue;
        enqueue(&ready_queues[init_queue], pid);
    }
}

bool scheduler_is_finished(Queue ready_queues[])
{
    return ready_queues[0].length == 0
        && ready_queues[1].length == 0
        && ready_queues[2].length == 0
        && ready_queues[3].length == 0; 
}

void scheduler_print_ready_queues(Queue ready_queues[])
{
    print_queue(&ready_queues[0]);
    print_queue(&ready_queues[1]);
    print_queue(&ready_queues[2]);
    print_queue(&ready_queues[3]);
}

ProcessId scheduler_dequeue_ready_queues(Queue ready_queues[])
{
    int queue_index;
    for (queue_index = 0; queue_index < 4; queue_index++)
    {
        if (ready_queues[queue_index].length > 0)
        {
            return dequeue(&ready_queues[queue_index]);
        }
    }
    return -1;
}
