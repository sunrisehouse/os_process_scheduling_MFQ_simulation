#include <stdio.h>
#include "input_reader/input_reader.h"
#include "scheduler/scheduler.h"

int main()
{
    printf("\n## OS Process Scheduling MFQ Simulation ##\n");

    printf("\n# 1. Read input\n");
    Input input = read_input("resource/test-input.txt");

    printf("    * number of process: %d\n", input.number_of_process);
    int process_index;
    for (process_index = 0; process_index < input.number_of_process; process_index++)
    {
        printf("    * (process %d) %d %d %d %d\t", process_index, input.process_inputs[process_index].id, input.process_inputs[process_index].arrival_time, input.process_inputs[process_index].init_queue, input.process_inputs[process_index].cycles);
        int cycle_index;
        for (cycle_index = 0; cycle_index < input.process_inputs[process_index].cycles - 1; cycle_index++)
        {
            printf("%d %d / ", input.process_inputs[process_index].cpu_burst_times[cycle_index], input.process_inputs[process_index].io_burst_times[cycle_index]);
        }
        printf("%d\n", input.process_inputs[process_index].cpu_burst_times[input.process_inputs[process_index].cycles - 1]);
    }

    printf("\n# 2. Scheduling\n");
    schedule(input);

    return 0;
}
