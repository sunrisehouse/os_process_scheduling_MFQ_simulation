#include <stdio.h>
#include "input_reader/input_reader.h"
#include "scheduler/scheduler.h"
#include "list.h"

typedef struct Gantt
{
    ProcessId process_id;
    SimulationTime arrival_time;
    SimulationTime in;
    SimulationTime out;
} Gantt;

typedef struct ProcessResult
{
    ProcessId process_id;
    SimulationTime total_turnaround_time;
    SimulationTime total_wating_time;
} ProcessResult;

void on_dispatch(ProcessId pid, SimulationTime time);
void on_finish_cpu_burst(ProcessId pid, SimulationTime arrival_time, SimulationTime in_time, SimulationTime out_time);
void on_preemtion(ProcessId pid, SimulationTime arrival_time, SimulationTime in_time, SimulationTime out_time);
void initialize_process_result(Input input);
void add_result(void* result, int index);
void print_input(Input input);
void print_gantt_chart();
void print_process_results();
void print_gantt(void* data, int i);
void print_process_result(void* data, int i);
void print_total_result();

List g_gantt_chart;
List g_process_results;

int main(int argc, char* argv[])
{
    if (argc != 2) return 1;
    char* input_file_name = argv[1];

    printf("\n## OS Process Scheduling MFQ Simulation ##\n");

    printf("\n# 1. Read input\n");
    printf("    file name: %s", input_file_name);
    Input input = read_input(input_file_name);

    print_input(input);

    printf("\n# 2. Scheduling\n");

    g_gantt_chart = create_list();
    g_process_results = create_list();

    initialize_process_result(input);

    schedule(
        input,
        on_dispatch,
        on_finish_cpu_burst,
        on_preemtion
    );

    printf("\n# 3. Result\n");

    print_gantt_chart();
    printf("\n");
    print_process_results();
    printf("\n");
    print_total_result();
    printf("\n");

    return 0;
}

void initialize_process_result(Input input)
{
    int i;
    for (i = 0; i < input.number_of_processes; i++)
    {
        ProcessResult* pr = (ProcessResult *) malloc(sizeof(ProcessResult));
        pr->process_id = input.process_inputs[i].id;
        pr->total_turnaround_time = 0;
        pr->total_wating_time = 0;
        list_push(&g_process_results, pr);
    }
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

    ListNode* current_node = g_process_results.front;
    for (current_node = g_process_results.front; current_node != NULL; current_node = current_node->next)
    {
        ProcessResult* pr = (ProcessResult *)current_node->data;
        if (pr->process_id == pid)
        {
            pr->total_turnaround_time += out_time - arrival_time;
            pr->total_wating_time += in_time - arrival_time;
            break;
        }
    }
}

void on_preemtion(ProcessId pid, SimulationTime arrival_time, SimulationTime in_time, SimulationTime out_time)
{
    Gantt* gt = (Gantt *) malloc(sizeof(Gantt));
    gt->process_id = pid;
    gt->arrival_time = arrival_time;
    gt->in = in_time;
    gt->out = out_time + 1;
    list_push(&g_gantt_chart, gt);

    ListNode* current_node = g_process_results.front;
    for (current_node = g_process_results.front; current_node != NULL; current_node = current_node->next)
    {
        ProcessResult* pr = (ProcessResult *)current_node->data;
        if (pr->process_id == pid)
        {
            pr->total_turnaround_time += out_time - arrival_time;
            pr->total_wating_time += in_time - arrival_time;
            break;
        }
    }
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

void print_gantt_chart()
{
    printf("    [Gantt Chart]\n");
    printf("      process\tarrival time\tdispatch time\n");
    list_for_each(&g_gantt_chart, print_gantt);
}

void print_process_results()
{
    printf("    [Procss Result]\n");
    printf("      process\tTT\tWT\n");
    list_for_each(&g_process_results, print_process_result);
}

void print_gantt(void* data, int i)
{
    Gantt* gt = (Gantt *) data;
    printf("      %d\t\t%d\t\t%d ~ %d\n", gt->process_id, gt->arrival_time, gt->in, gt->out);
}

void print_process_result(void* data, int i)
{
    ProcessResult* pr = (ProcessResult *) data;
    printf("      %d\t\t%d\t%d\n", pr->process_id, pr->total_turnaround_time, pr->total_wating_time);
}

void print_total_result()
{
    float total_tt = 0.0f;
    float total_wt = 0.0f;

    ListNode* current_node = g_process_results.front;
    for (current_node = g_process_results.front; current_node != NULL; current_node = current_node->next)
    {
        ProcessResult* pr = (ProcessResult *)current_node->data;
        total_tt += pr->total_turnaround_time;
        total_wt += pr->total_wating_time;
    }

    printf("    [Total Result]\n");
    printf("      average turnaround time: %.3f\n", total_tt / list_get_length(&g_process_results));
    printf("      average wating time: %.3f\n", total_wt / list_get_length(&g_process_results));
}
