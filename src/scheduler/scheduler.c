#include "scheduler.h"

void schedule(Input input)
{
    MFQScheduler scheduler;
    scheduler_init(input, &scheduler);

    // scheduler_print_processes(scheduler);

    SimulationTime time = 0;
    while (!scheduler_is_finished(scheduler))
    {
        if (time % 1 == 0)
        {
            printf("input any character: ");
            char a[10];
            scanf("%c", a);

            if (a[0] == 'x') break;
        }
        printf("*****[%d] cycle************\n", time);
        scheduler_push_process(time, &scheduler);

        scheduler_print_ready_queues(scheduler);

        if (scheduler.procerss_in_cpu == NULL)
        {
            scheduler_dispatch_process(&scheduler);            
        }

        // io
        scheduler_process_io(&scheduler);

        // cpu
        if (scheduler.procerss_in_cpu != NULL)
        {
            scheduler_process_cpu(&scheduler);          
        }

        if (scheduler.procerss_in_cpu != NULL)
        {
            scheduler_after_cpu(time, &scheduler);       
        }
        scheduler_after_io(time, &scheduler);

        scheduler_print_processes(scheduler);
        time++;
    }
}

void scheduler_init(Input input, MFQScheduler* scheduler) {
    int i;
    for (i = 0; i < MFQ_READY_QUEUE_SIZE; i++)
    {
        scheduler->ready_queues[i] = create_queue();
    }

    scheduler->number_of_processes = input.number_of_processes;
    scheduler->processes = (Process *) malloc(sizeof(Process) * scheduler->number_of_processes);

    scheduler->procerss_in_cpu = NULL;
    scheduler->preemtion_timer = 0;

    for (i = 0; i < scheduler->number_of_processes; i++)
    {
        ProcessInput pi = input.process_inputs[i];
        scheduler->processes[i].id = pi.id;
        scheduler->processes[i].arrival_time = pi.arrival_time;
        scheduler->processes[i].queue_id = pi.init_queue;
        scheduler->processes[i].is_in_io = false;
        scheduler->processes[i].cpu_times = create_queue();
        scheduler->processes[i].io_times = create_queue();

        int j;
        for (j = 0; j < pi.cycles; j++)
        {
            enqueue(&scheduler->processes[i].cpu_times, pi.cpu_burst_times[j]);
        }

        for (j = 0; j < pi.cycles - 1; j++)
        {
            enqueue(&scheduler->processes[i].io_times, pi.io_burst_times[j]);
        }
    }
}

void scheduler_push_process(SimulationTime time, MFQScheduler* scheduler)
{
    int number_of_processes = scheduler->number_of_processes;
    int i;
    for (i = 0; i < number_of_processes; i++)
    {
        Process pr = scheduler->processes[i];
        if (pr.arrival_time == time && get_length(pr.cpu_times) > 0)
        {
            printf("process (%d) go to ready queue\n", pr.id);
            enqueue(&scheduler->ready_queues[pr.queue_id], pr.id);
        }
    }
}

void scheduler_dispatch_process(MFQScheduler* scheduler)
{
    int i;
    for (i = 0; i < MFQ_READY_QUEUE_SIZE; i++)
    {
        if (get_length(scheduler->ready_queues[i]) == 0) continue;
        ProcessId pid = dequeue(&scheduler->ready_queues[i]);
        int j;
        for (j = 0; j < scheduler->number_of_processes; j++)
        {
            if (scheduler->processes[j].id == pid)
            {
                scheduler->procerss_in_cpu = &scheduler->processes[j];
                break;
            }
        }

        if (scheduler->procerss_in_cpu != NULL) break;
    }

    if (scheduler->procerss_in_cpu == NULL) return;
    
    printf("process (%d) pop ready queues\n", scheduler->procerss_in_cpu->id);

    if (scheduler->procerss_in_cpu->queue_id == 0)
    {
        scheduler->preemtion_timer = 1;
    }
    else if (scheduler->procerss_in_cpu->queue_id == 1)
    {
        scheduler->preemtion_timer = 2;
    }
    else if (scheduler->procerss_in_cpu->queue_id == 2)
    {
        scheduler->preemtion_timer = 4;
    }
    else if (scheduler->procerss_in_cpu->queue_id == 3)
    {
    }
    printf("process (%d) is dispatched with %s, %d\n", scheduler->procerss_in_cpu->id, scheduler_get_current_scheduler_technique(*scheduler) == RR ? "RR" : "FCFR", scheduler->procerss_in_cpu->cpu_times.front->data);
}

void scheduler_process_io(MFQScheduler* scheduler) {
    int i;
    for (i = 0; i < scheduler->number_of_processes; i++)
    {
        if (scheduler->processes[i].is_in_io && get_length(scheduler->processes[i].io_times) > 0)
        {
            scheduler->processes[i].io_times.front->data -= 1;
            printf("process (%d) do io, io time: %d \n", scheduler->processes[i].id, scheduler->processes[i].io_times.front->data);
        }
    }
}

void scheduler_process_cpu(MFQScheduler* scheduler) {
    printf("process (%d) do cpu: ", scheduler->procerss_in_cpu->id);
    if (scheduler_get_current_scheduler_technique(*scheduler) == RR)
    {
        scheduler->preemtion_timer -= 1;
        scheduler->procerss_in_cpu->cpu_times.front->data -= 1;
        printf("RR preemtion timer: %d, cpu times %d \n", scheduler->preemtion_timer, scheduler->procerss_in_cpu->cpu_times.front->data);
    }
    else if (scheduler_get_current_scheduler_technique(*scheduler) == FCFR)
    {
        scheduler->procerss_in_cpu->cpu_times.front->data -= 1;
        printf("FCFR preemtion timer: %d\n", scheduler->preemtion_timer);
    }
}

void scheduler_after_cpu(SimulationTime time, MFQScheduler* scheduler)
{
    if (scheduler->procerss_in_cpu->cpu_times.front->data == 0)
    {
        // cpu burst 끝 io 시작
        printf("process (%d) cpu burst end\n", scheduler->procerss_in_cpu->id);
        scheduler->procerss_in_cpu->is_in_io = true;
        if (scheduler->procerss_in_cpu->queue_id > 0 && scheduler->procerss_in_cpu->queue_id < 2)
        {
            scheduler->procerss_in_cpu->queue_id -= 1;
        }
        dequeue(&scheduler->procerss_in_cpu->cpu_times);
        scheduler->procerss_in_cpu = NULL;
    }
    else if (scheduler_get_current_scheduler_technique(*scheduler) == RR && scheduler->preemtion_timer == 0)
    {
        // preemtion 해야함
        printf("process (%d) preemtion\n", scheduler->procerss_in_cpu->id);
        scheduler->procerss_in_cpu->arrival_time = time + 1;
        if (scheduler->procerss_in_cpu->queue_id < 2)
        {
            scheduler->procerss_in_cpu->queue_id += 1;
        }
        scheduler->procerss_in_cpu = NULL;
    }
}

void scheduler_after_io(SimulationTime time, MFQScheduler* scheduler)
{
    int i;
    for (i = 0; i < scheduler->number_of_processes; i++)
    {
        if (scheduler->processes[i].is_in_io)
        {
            printf("process (%d) io end\n", scheduler->processes[i].id);

            if (get_length(scheduler->processes[i].io_times) > 0 && scheduler->processes[i].io_times.front->data == 0)
            {
                dequeue(&scheduler->processes[i].io_times);
                scheduler->processes[i].arrival_time = time + 1;
                scheduler->processes[i].is_in_io = false;
            }
            else if (get_length(scheduler->processes[i].io_times) == 0)
            {
                scheduler->processes[i].is_in_io = false;
            }
        }
    }
}

bool scheduler_is_finished(MFQScheduler scheduler)
{
    int i;
    for (i = 0; i < scheduler.number_of_processes; i++)
    {
        if (get_length(scheduler.processes[i].cpu_times) > 0) return false;
    }
    return true;
}

SchedulerTechnique scheduler_get_current_scheduler_technique(MFQScheduler scheduler)
{
    return QUEUE_SCHEDULER_TECHNIQUES[scheduler.procerss_in_cpu->queue_id];
}

void scheduler_print_ready_queues(MFQScheduler scheduler)
{
    printf("Ready Queue\n");
    int i;
    for (i = 0; i < MFQ_READY_QUEUE_SIZE; i++)
    {
        printf("  ");
        print_queue(scheduler.ready_queues[i]);
    }
    printf("\n\n");
}

void scheduler_print_processes(MFQScheduler scheduler)
{
    printf("\nProcesses\n");
    int i;
    for (i = 0; i < scheduler.number_of_processes; i++)
    {
        printf("  process (%d)\n", scheduler.processes[i].id);
        printf("    cpu: ");
        print_queue(scheduler.processes[i].cpu_times);
        printf("    io: ");
        print_queue(scheduler.processes[i].io_times);
        printf("    arrival time: %d\n", scheduler.processes[i].arrival_time);
        printf("    queue id: %d\n", scheduler.processes[i].queue_id);
        printf("    is in io?: %s\n", scheduler.processes[i].is_in_io ? "true" : "false");
    }
    printf("\n");
}
