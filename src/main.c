#include <stdio.h>
#include "input_reader/input_reader.h"
#include "scheduler/scheduler.h"

void print_input(Input input);

int main()
{
    printf("\n## OS Process Scheduling MFQ Simulation ##\n");

    printf("\n# 1. Read input\n");
    Input input = read_input("resource/test-input.txt");

    print_input(input);

    printf("\n# 2. Scheduling\n");
    schedule(input);

    return 0;
}

void print_input(Input input)
{
    printf("    * number of process: %d\n", input.number_of_processes);
    int process_index;
    for (process_index = 0; process_index < input.number_of_processes; process_index++)
    {
        printf("    * (process %d) %d %d %d %d\t", process_index, input.process_inputs[process_index].id, input.process_inputs[process_index].arrival_time, input.process_inputs[process_index].init_queue, input.process_inputs[process_index].cycles);
        int cycle_index;
        for (cycle_index = 0; cycle_index < input.process_inputs[process_index].cycles - 1; cycle_index++)
        {
            printf("%d %d / ", input.process_inputs[process_index].cpu_burst_times[cycle_index], input.process_inputs[process_index].io_burst_times[cycle_index]);
        }
        printf("%d\n", input.process_inputs[process_index].cpu_burst_times[input.process_inputs[process_index].cycles - 1]);
    }
}
