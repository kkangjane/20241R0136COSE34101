#include "scheduler.h"

static int	Do_CPU(int real);

void	FCFS_scheduling()
{
	int cpu_check, io_check, t;
	int	cpu_chart[10000], io_chart[10000];

	sum_time(&cpu_check, &io_check);
	srand((unsigned)time(0));
	
	t = 0;
	while (1)
	{
        arrive_process(t);
		if (terminate_check(t))
			break;

		cpu_chart[t] = Do_CPU(0);
		if (cpu_chart[t] >= 0)
			cpu_check -= 1;
		
		io_chart[t] = Do_IO(0);
		if (io_chart[t] >= 0)
			io_check -= 1;

		update_waiting_time();
		random_io_out();
		t++;
	}
	print_result(cpu_chart, io_chart, cpu_check, io_check, t);
}

static int	Do_CPU(int real)
{
	srand((unsigned)time(0));
	if (real)
	{
		if (running_cpu > -1)
			p_list[running_cpu].CPU_rest -= 1;
		return running_cpu;
	}
	
	// 1. ready_queue에서 가져오기.
	if (running_cpu == -1)
	{
		// 1.1. ready_queue empty면 CPU 쉼
		if (ready_queue.size == 0)
			return Do_CPU(1);
		running_cpu = queue_pop(&ready_queue);
		// 1.2. 가져온 게 CPU=1이고, I/O>0이면 waiting_queue로 보내고, 다시 Do_CPU(0)
		if (p_list[running_cpu].IO_rest && p_list[running_cpu].CPU_rest == 1)
		{
			queue_push(&waiting_queue, running_cpu);
			running_cpu = -1;
			return Do_CPU(0);
		}
		// 1.3. 가져온게 멀쩡하면 Do_CPU(1)
		return Do_CPU(1);
	}
	
	// 2. 기존 프로세스 실행하기
	// 2.1. IO_rest=0이면 Do_CPU(1)
	if (p_list[running_cpu].IO_rest == 0)
		return Do_CPU(1);

	// 2.2. cpu_rest=1이면 I/O 남은거 실행해야 하므로 waiting_queue에 보내고, 다시 Do_CPU(0)
	if (p_list[running_cpu].CPU_rest == 1)
	{
		queue_push(&waiting_queue, running_cpu);
		running_cpu = -1;
		return Do_CPU(0);
	}
	// 2.3. cpu_rest 넉넉한데 ready_queue 비어있으면 Do_CPU(1)
	if (ready_queue.size == 0)
		return Do_CPU(1);

	// 2.4. 다른 프로세스들이 기다린다면 랜덤하게 I/O로 보내고 Do_CPU(0), 아니면 Do_CPU(1)
	if (rand() % 2 && waiting_queue.size < 1)
	{
		queue_push(&waiting_queue, running_cpu);
		running_cpu = -1;
		return Do_CPU(0);
	}
	return Do_CPU(1);
}
