#ifndef _TYPES_HEADER_FILE
#define _TYPES_HEADER_FILE

typedef int ProcessId;
typedef int Time;
typedef int QueueId;

typedef struct ProcessInput {
    ProcessId id;
    Time arrival_time;
    QueueId init_queue;
    int cycles;
    Time* cpu_burst_times;
    Time* io_burst_times;
} ProcessInput;

typedef struct Input {
    int number_of_process;
    ProcessInput* process_inputs;
} Input;

typedef struct MFQScheduler {
    ProcessId* ready_queue[4];
} MFQScheduler;

#endif
