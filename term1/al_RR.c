#include "scheduler.h"

static int	Do_CPU(int real);

void	RR_scheduling()
{
	int t = 0;
	int cpu_chart[10000];
	int	io_chart[10000];
	int cpu_check, io_check;
	sum_time(&cpu_check, &io_check);

	srand((unsigned)time(0));
	while (1)
	{
		arrive_process(t);
		terminate_check(t);
		if (terminated_queue.size == num)
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
		{
			// CPU_rest를 1 줄이고, rr_time(time quantum)을 1 늘린다.
			p_list[running_cpu].CPU_rest -= 1;
			p_list[running_cpu].rr_time += 1;
		}
		return running_cpu;
	}

	// 1. ready_queue에서 가져오기.
	if (running_cpu == -1)
	{
		// 1.1. ready_queue empty면 CPU 쉼
		if (ready_queue.size == 0)
			return Do_CPU(1);
		// ready_queue에서 프로세스를 pop
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
	// 2.1. IO_rest 남고, CPU_rest 1이면 무조건 waiting_queue에 보내고 새로운 프로세스 가져오기
	if (p_list[running_cpu].CPU_rest == 1 && p_list[running_cpu].IO_rest)
	{
		p_list[running_cpu].rr_time = 0;
		queue_push(&waiting_queue, running_cpu);
		running_cpu = -1;
		return Do_CPU(0);
	}
	
	// 2.2. quantum time 2로 채우면 ready나 waiting queue로 보내고 새로운 프로세스 가져오기
	if (p_list[running_cpu].rr_time == 2)
	{
		if (rand() % 2 && waiting_queue.size < 1 && p_list[running_cpu].IO_rest)
			queue_push(&waiting_queue, running_cpu);
		else
			queue_push(&ready_queue, running_cpu);
		p_list[running_cpu].rr_time = 0;
		running_cpu = -1;
		return Do_CPU(0);
	}
	// 2.3. 기존 프로세스 CPU 계속 유지
	return Do_CPU(1);
}
