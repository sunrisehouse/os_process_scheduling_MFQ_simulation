#include <stdio.h>
#include "input_reader/input_reader.h"
#include "scheduler/scheduler.h"
#include "list.h"

typedef struct Gantt {
    ProcessId process_id;
    SimulationTime arrival_time;
    SimulationTime in;
    SimulationTime out;
} Gantt;

void print_input(Input input);
void print_gantt(void* data, int i);
void on_dispatch(ProcessId pid, SimulationTime time);
void on_finish_cpu_burst(ProcessId pid, SimulationTime arrival_time, SimulationTime in_time, SimulationTime out_time);
void on_preemtion(ProcessId pid, SimulationTime arrival_time, SimulationTime in_time, SimulationTime out_time);

List g_gantt_chart;

int main()
{
    printf("\n## OS Process Scheduling MFQ Simulation ##\n");

    printf("\n# 1. Read input\n");
    Input input = read_input("resource/test-input.txt");

    print_input(input);

    printf("\n# 2. Scheduling\n");

    g_gantt_chart = create_list();

    schedule(
        input,
        on_dispatch,
        on_finish_cpu_burst,
        on_preemtion
    );

    printf("    process\tarrival time\tdispatch time\n");
    list_for_each(&g_gantt_chart, print_gantt);

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

void print_gantt(void* data, int i)
{
    Gantt* gt = (Gantt *) data;
    printf("    %d\t\t%d\t\t%d ~ %d\n", gt->process_id, gt->arrival_time, gt->in, gt->out);
}

void on_dispatch(ProcessId pid, SimulationTime time)
{
}

void on_finish_cpu_burst(ProcessId pid, SimulationTime arrival_time, SimulationTime in_time, SimulationTime out_time)
{
    Gantt* gt = (Gantt *) malloc(sizeof(Gantt));
    gt->process_id = pid;
    gt->arrival_time = arrival_time;
    gt->in = in_time;
    gt->out = out_time + 1;
    list_push(&g_gantt_chart, gt);
}

void on_preemtion(ProcessId pid, SimulationTime arrival_time, SimulationTime in_time, SimulationTime out_time)
{
    Gantt* gt = (Gantt *) malloc(sizeof(Gantt));
    gt->process_id = pid;
    gt->arrival_time = arrival_time;
    gt->in = in_time;
    gt->out = out_time + 1;
    list_push(&g_gantt_chart, gt);
}
