#ifndef _SCHEDULER_HEADER_FILE
#define _SCHEDULER_HEADER_FILE

#include <stdbool.h>

#include "../types.h"
#include "../queue.h"


typedef struct Process {
    ProcessId id;
    Queue cpu_times;
    Queue io_times;
    SimulationTime arrival_time;
    QueueId queue_id;
    bool is_in_io;
} Process;

typedef enum SchedulerTechnique {
    RR,
    FCFR,
} SchedulerTechnique;

static const int MFQ_READY_QUEUE_SIZE = 4;
static const SchedulerTechnique QUEUE_SCHEDULER_TECHNIQUES[MFQ_READY_QUEUE_SIZE] = {
    RR, RR, RR, FCFR
};

typedef struct MFQScheduler {
    int number_of_processes;
    Process* processes;
    Queue ready_queues[MFQ_READY_QUEUE_SIZE];

    Process* procerss_in_cpu;
    SimulationTime preemtion_timer;
} MFQScheduler;

void schedule(Input input);
void scheduler_init(Input input, MFQScheduler* scheduler);
void scheduler_push_process(SimulationTime time, MFQScheduler* scheduler);
void scheduler_dispatch_process(MFQScheduler* scheduler);
bool scheduler_is_finished(MFQScheduler scheduler);
void scheduler_print_ready_queues(MFQScheduler scheduler);
void scheduler_print_processes(MFQScheduler scheduler);
void scheduler_burst_io(MFQScheduler* scheduler);
void scheduler_burst_cpu(MFQScheduler* scheduler);
void scheduler_after_cpu(SimulationTime time, MFQScheduler* scheduler);
void scheduler_after_io(SimulationTime time, MFQScheduler* scheduler);
SchedulerTechnique scheduler_get_current_scheduler_technique(MFQScheduler scheduler);

#endif
