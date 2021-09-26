#ifndef _TYPES_HEADER_FILE
#define _TYPES_HEADER_FILE

typedef int ProcessId;
typedef int ArrivalTime;
typedef int InitQueue;
typedef int BurstTime;

typedef struct ProcessInput {
    ProcessId id;
    ArrivalTime arrival_time;
    InitQueue init_queue;
    int cycles;
    BurstTime* cpu_burst_times;
    BurstTime* io_burst_times;
} ProcessInput;

typedef struct Input {
    int number_of_process;
    ProcessInput* process_inputs;
} Input;

typedef struct MFQScheduler {
    ProcessId* ready_queue[4];
} MFQScheduler;

#endif
