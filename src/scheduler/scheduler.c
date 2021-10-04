#include "scheduler.h"

void schedule(
    Input input,
    void on_dispatch(ProcessId, SimulationTime),
    void on_finish_cpu_burst(ProcessId pid, SimulationTime time),
    void on_preemtion(ProcessId pid, SimulationTime time)
)
{
    MFQScheduler scheduler;
    scheduler_init(input, &scheduler);

    // scheduler_print_processes(scheduler);

    while (!scheduler_is_finished(scheduler))
    {
        #ifdef DEBUG
        if (time % 1 == 0)
        {
            printf("input any character: ");
            char a[10];
            scanf("%c", a);

            if (a[0] == 'x') break;
        }
        #endif

        #ifdef DEBUG
        printf("--------[%d cycle]--------\n\n", time);
        #endif

        scheduler_push_process(&scheduler);

        #ifdef DEBUG
        scheduler_print_ready_queues(scheduler);
        #endif

        if (scheduler.procerss_in_cpu == NULL)
        {
            scheduler_dispatch_process(&scheduler, on_dispatch);
        }

        // io
        scheduler_burst_io(&scheduler);

        // cpu
        if (scheduler.procerss_in_cpu != NULL)
        {
            scheduler_burst_cpu(&scheduler); 
        }

        if (scheduler.procerss_in_cpu != NULL)
        {
            scheduler_after_cpu(&scheduler, on_finish_cpu_burst, on_preemtion);
        }
        scheduler_after_io(&scheduler);


        #ifdef DEBUG
        printf("\n");
        scheduler_print_processes(scheduler);
        #endif

        scheduler.time++;
    }
}

void scheduler_init(Input input, MFQScheduler* scheduler) {
    int i;
    for (i = 0; i < MFQ_READY_QUEUE_SIZE; i++)
    {
        scheduler->ready_queues[i] = create_queue();
    }
    scheduler->time = 0;
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

void scheduler_push_process(MFQScheduler* scheduler)
{
    int number_of_processes = scheduler->number_of_processes;
    int i;
    for (i = 0; i < number_of_processes; i++)
    {
        Process pr = scheduler->processes[i];
        if (pr.arrival_time == scheduler->time && get_length(pr.cpu_times) > 0)
        {
            #ifdef DEBUG
            printf("[push ready queue] process (%d)\n", pr.id);
            #endif
            enqueue(&scheduler->ready_queues[pr.queue_id], pr.id);
        }
    }
}

void scheduler_dispatch_process(MFQScheduler* scheduler, void on_dispatch(ProcessId, SimulationTime))
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
                on_dispatch(scheduler->procerss_in_cpu->id, scheduler->time);
                #ifdef DEBUG
                printf("[dispatch] process (%d) with %s, %d\n", scheduler->procerss_in_cpu->id, scheduler_get_current_scheduler_technique(*scheduler) == RR ? "RR" : "FCFR", scheduler->procerss_in_cpu->cpu_times.front->data);
                #endif
                return;
            }
        }
    }
}

void scheduler_burst_io(MFQScheduler* scheduler) {
    int i;
    for (i = 0; i < scheduler->number_of_processes; i++)
    {
        if (scheduler->processes[i].is_in_io && get_length(scheduler->processes[i].io_times) > 0)
        {
            scheduler->processes[i].io_times.front->data -= 1;
            #ifdef DEBUG
            printf("[io burst] process (%d) io time: %d \n", scheduler->processes[i].id, scheduler->processes[i].io_times.front->data);
            #endif
        }
    }
}

void scheduler_burst_cpu(MFQScheduler* scheduler) {
    if (scheduler_get_current_scheduler_technique(*scheduler) == RR)
    {
        scheduler->preemtion_timer -= 1;
        scheduler->procerss_in_cpu->cpu_times.front->data -= 1;
        #ifdef DEBUG
        printf("[cpu burst] process (%d) RR preemtion timer: %d, cpu times %d \n", scheduler->procerss_in_cpu->id, scheduler->preemtion_timer, scheduler->procerss_in_cpu->cpu_times.front->data);
        #endif
    }
    else if (scheduler_get_current_scheduler_technique(*scheduler) == FCFR)
    {
        scheduler->procerss_in_cpu->cpu_times.front->data -= 1;
        #ifdef DEBUG
        printf("[cpu burst] process (%d) FCFR preemtion timer: %d\n", scheduler->procerss_in_cpu->id, scheduler->preemtion_timer);
        #endif
    }
}

void scheduler_after_cpu(
    MFQScheduler* scheduler,
    void on_finish_cpu_burst(ProcessId pid, SimulationTime time),
    void on_preemtion(ProcessId pid, SimulationTime time)
)
{
    Process* pr = scheduler->procerss_in_cpu;
    if (pr->cpu_times.front->data == 0)
    {
        #ifdef DEBUG
        printf("[cpu burst end] process (%d)\n", pr->id);
        #endif
        dequeue(&pr->cpu_times);
        if (get_length(pr->io_times) > 0)
        {
            pr->is_in_io = true;
            if (pr->queue_id > 0 && pr->queue_id < 2)
            {
                pr->queue_id -= 1;
            }
        }
        scheduler->procerss_in_cpu = NULL;
        on_finish_cpu_burst(pr->id, scheduler->time);
    }
    else if (scheduler_get_current_scheduler_technique(*scheduler) == RR && scheduler->preemtion_timer == 0)
    {
        #ifdef DEBUG
        printf("[preemtion] process (%d)\n", pr->id);
        #endif
        pr->arrival_time = scheduler->time + 1;
        if (pr->queue_id < 2)
        {
            pr->queue_id += 1;
        }
        scheduler->procerss_in_cpu = NULL;
        on_preemtion(pr->id, scheduler->time);
    }
}

void scheduler_after_io(MFQScheduler* scheduler)
{
    int i;
    for (i = 0; i < scheduler->number_of_processes; i++)
    {
        if (scheduler->processes[i].is_in_io)
        {
            if (get_length(scheduler->processes[i].io_times) > 0 && scheduler->processes[i].io_times.front->data == 0)
            {
                #ifdef DEBUG
                printf("[IO end]: process(%d)\n", scheduler->processes[i].id);
                #endif
                dequeue(&scheduler->processes[i].io_times);
                scheduler->processes[i].arrival_time = scheduler->time + 1;
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
