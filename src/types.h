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
