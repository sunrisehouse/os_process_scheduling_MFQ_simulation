#ifndef _TYPES_HEADER_FILE
#define _TYPES_HEADER_FILE

typedef int ProcessId;
typedef int SimulationTime;
typedef int QueueId;

typedef struct ProcessInput {
    ProcessId id;
    SimulationTime arrival_time;
    QueueId init_queue;
    int cycles;
    SimulationTime* cpu_burst_times;
    SimulationTime* io_burst_times;
} ProcessInput;

typedef struct Input {
    int number_of_processes;
    ProcessInput* process_inputs;
} Input;

typedef struct Gantt {
    ProcessId process_id;
    SimulationTime in;
    SimulationTime out;
} Gantt;

typedef struct Result {
    Gantt* gantt_chart;
} Result;

#endif
