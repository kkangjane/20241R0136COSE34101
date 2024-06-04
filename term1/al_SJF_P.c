#include "scheduler.h"

static int	Do_CPU(int real);

void	Preemptive_SJF_scheduling()
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
		if (running_cpu >= 0)
			p_list[running_cpu].CPU_rest -= 1;
		return running_cpu;
	}

	// 1. ready_queue에서 가져오기.
	if (running_cpu == -1)
	{
		// 1.1. ready_queue empty면 CPU 쉼
		if (ready_queue.size == 0)
			return Do_CPU(1);
		// ready_queue에서 CPU_rest가 가장 작은 프로세스를 pop
		running_cpu = queue_pop_SJF(1);
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
	// 2.1. CPU_rest=1이고 IO_rest 남으면 무조건 waiting_queue로 보내고 새로운 프로세스 가져오기
	if (p_list[running_cpu].CPU_rest == 1 && p_list[running_cpu].IO_rest)
	{
		queue_push(&waiting_queue, running_cpu);
		running_cpu = -1;
		return Do_CPU(0);
	}

	// 2.2. 기존 프로세스의 cpu_rest랑 ready_queue랑 다시 비교
	int min = queue_pop_SJF(0);
	// 만약 ready_queue에 있는게 CPU_rest가 적으면 기존은 waiting이나 ready_queue에 보내고 Do_CPU(0)
	if (p_list[running_cpu].CPU_rest > p_list[min].CPU_rest)
	{
		if (p_list[running_cpu].IO_rest)
			queue_push(&waiting_queue, running_cpu);
		else
			queue_push(&ready_queue, running_cpu);
		running_cpu = -1;
		return Do_CPU(0);
	}
	// 2.3. 기존이 우선순위가 가장 높으면 random한 확률로 I/O 실행하게 하기
	if (p_list[running_cpu].IO_rest && waiting_queue.size < 1 && rand() % 2)
	{
		queue_push(&waiting_queue, running_cpu);
		running_cpu = -1;
		return Do_CPU(0);
	}
	// 2.4. 기존 실행
	return Do_CPU(1);
}
