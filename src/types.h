#ifndef _TYPES_HEADER_FILE
#define _TYPES_HEADER_FILE

typedef struct ProcessInput {
    int id;
    int arrival_time;
    int init_queue;
    int cycles;
    int* cpu_burst_times;
    int* io_burst_times;
} ProcessInput;

typedef struct Input {
    int number_of_process;
    ProcessInput* process_inputs;
} Input;

typedef int ProcessId;

typedef struct MFQScheduler {
    ProcessId* ready_queue[4];
} MFQScheduler;

#endif
