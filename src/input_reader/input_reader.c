#include "input_reader.h"

Input read_input(char* file_name) {
    const int number_of_fixed_attributes = 4;
    const int buffer_size = 100;

    Input input;
    FILE *fp = fopen(file_name, "r");

    fscanf(fp, "%d", &input.number_of_process);
    input.process_inputs = (ProcessInput*) malloc(sizeof(ProcessInput) * input.number_of_process);

    int index_of_process;
    for (index_of_process = 0; index_of_process < input.number_of_process; index_of_process++)
    {
        ProcessInput process_input;
        fscanf(fp, "%d %d %d %d", &process_input.id, &process_input.arrival_time, &process_input.init_queue, &process_input.cycles);
        process_input.cpu_burst_times = (int *) malloc(sizeof(int) * process_input.cycles);
        process_input.io_burst_times = (int *) malloc(sizeof(int) * (process_input.cycles - 1));


        if (process_input.cycles > 0) {
            int index_of_cycles;
            for (index_of_cycles = 0; index_of_cycles < process_input.cycles - 1; index_of_cycles++)
            {
                fscanf(fp, "%d %d", &process_input.cpu_burst_times[index_of_cycles], &process_input.io_burst_times[index_of_cycles]);
            }
            fscanf(fp, "%d", &process_input.cpu_burst_times[index_of_cycles]);
        }
        
        input.process_inputs[index_of_process] = process_input;
    }

    return input;
}
