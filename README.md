# OS Process Scheduling MFQ Simulation

## Objective

MFQ 스케줄링 기법 구현

4 개의 Ready Queue 를 갖는 MFQ Scheduling 기법 구현
- Ready Queue 0: time quantum 1 인 RR 스케줄링 기법 사용
- Ready Queue 1: time quantum 2 인 RR 스케줄링 기법 사용
- Ready Queue 2: time quantum 4 인 RR 스케줄링 기법 사용
- Ready Queue 3: FCFS 스케줄링 기법 사용
- Ready Queue 의 우선 순위는 Q0 > Q1 > Q2 > Q3 이다.

프로세스는 최초에 각자에게 지정된 Queue 에 진입하게된다.

IO burst 가 끝나고 나서는 상위 우선순위의 Ready Queue 로 진입한다. (Q0 에서는 불가)

Cpu burst time 이 끝나기 전에 time quantum 이 끝나게 된 프로세스는 하위 우선순위의 Ready Queue 로 진입한다. (Q3 에서는 불가)

## 개발 환경

Programming language: C

Compiler: gcc

make 와 makefile 을 이용해 빌드했습니다.

## Build / Execution

build 는 프로젝트 폴더의 root 에서 make 명령어 실행
```
make
```

빌드를 하게 되면 "/build" 폴더에 simulation 실행파일이 생긴다. 그 실행파일을 원하는 input 파일을 argument 로 줘서 실행한다.

```
./build/simulation resource/test-input-1.txt
```

## Data Structure

```cpp

typedef struct Process {
    ProcessId id;
    Queue cpu_times;
    Queue io_times;
    SimulationTime arrival_time;
    SimulationTime initial_arrival_time;
    QueueId queue_id;
    bool is_in_io;
} Process;

typedef struct MFQScheduler {
    int number_of_processes;
    Process* processes;
    Queue ready_queues[MFQ_READY_QUEUE_SIZE];

    Process* procerss_in_cpu;
    SimulationTime preemtion_timer;
    SimulationTime time;
    SimulationTime dispatched_itme;
} MFQScheduler;

```

## Scheduler Implementation

사이클 마다 다음의 로직을 수행하며 시뮬레이션을 진행합니다. 모든 프로세스들의 cpu burst time 들이 다 소모가 되면 시뮬레이션이 종료됩니다.

- scheduler_push_process function

    각 사이클에서 제일 먼저 하는 일은 도착한 Process 들을 레디큐에 넣습니다. Process 에 있는 arrival_time 과 queue_id 를 통해 자신의 ready queue 에 들어가게 됩니다. 이 때 arrival_time 이 같아서 누가 먼저 ready queue 에 들어가는지는 input 으로 들어온 프로세스 순서에 따라서로 가정했습니다. 또한 Process 들의 id 는 Unique 하다고 가정했습니다.

- scheduler_dispatch_process function
    
    그 후 스케줄러는 CPU 가 비어있는지 검사를 하고 비어있다면 Ready Queue 에서 프로세스를 dispatch 시킵니다. 이 때 MFQScheduler 안에 process_in_cpu 가 NULL 인지 검사를 해서 비어있는지 확인하고 비어있다면 MFQScheduler 안에 ready_queues 에서 제일 우선순위가 높은 레디큐에서 부터 찾아보면서 있다면 그 프로세스를 dequeue 해서 process_in_cpu 에 저장합니다. 그리고 어디 ready queue 에서 나왔는지를 보고 preemption timer 를 초기화 합니다.

- scheduler_burst_io function, scheduler_burst_cpu function

    그 다음 io 작업과 cpu 작업을 시작하게 됩니다. io 작업은 현재 들어온 모든 process 들을 검사해 is_in_io 가 true 인 것들의 io_times 의 맨 앞 시간을 1 만큼 감소시켜서 표현했습니다. cpu 작업은 MFQScheduler 의 procerss_in_cpu 에 있는 process 의 cpu_times 의 맨 앞 시간을 1 만큼 감소시켜 표현했습니다. 이 때 process 가 어디 ready queue 를 배정받았는지를 보고 preemtion timer 를 1 만큼 감소시킬지 말지를 결정합니다.

- scheduler_after_cpu function

    io 와 cpu 작업이 끝난 후 이제 끝난 작업에 대한 처리를 하게 됩니다. cpu 작업을 마치고 cpu burst time 을 다 끝내게 됐다면 해당 프로세스를 cpu 에서 꺼내고 io 를 하도록 보냅니다. MFQScheduler 의 process_in_cpu 를 NULL 로 만들고 is_in_io 를 true 로 만듭니다. 그리고 queue id 를 1 감소시켜 우선순위를 높여줍니다. cpu burst time 이 끝나지 않았고 해당 프로세스가 FCFS ready queue 에서 나왔다면 preemption timer 가 끝났는지 검사합니다. preemption timer 가 0 이 됐다면 해당 프로세스의 queue id 를 1 증가시켜 우선순위를 낮추고 다음 번 싸이클 때 레디큐에 등록해주기 위해 arrival time 을 현재 시간에 + 1 해서 저장합니다.

- scheduler_after_io function

    io 작업을 마치고 모든 프로세스들을 검사해 io 작업 중인 것들을 대상으로 io times 가 0 이 됐다면 다음 시간부터 cpu 작업을 하러가기 위한 레디큐 등록을 위해 is_in_io 를 false 로 바꿔주고 arrival time 을 현재 시간에 + 1 해서 저장합니다.


## 기타 구현

* Doubly Linked List 구현 (list.h list.c)
* Doubly Linked List 로 Queue 구현 (queue.h queue.c)
    - Ready Queue 구현
    - 스케쥴러안 프로세스들의 cpu times 와 io times Queue 로 저장
* input 을 입력 받기 위한 Input Reader 구현 (input_reader.h input_reader.c)
