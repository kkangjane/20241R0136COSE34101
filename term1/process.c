#include "scheduler.h"

void	Create_Process(void)
{
	srand((unsigned)time(0));
	make_pid();
	for (int i = 0; i < num; i++)
	{
		p_list[i].CPU_burst_time = rand() % 5 + 2;
		p_list[i].IO_burst_time = rand() % 5;
		p_list[i].arrival_time = rand() % 10;
		p_list[i].priority = rand() % 10;
		p_list[i].waiting_time = 0;
	}
	print_processes();
}

void	make_pid(void)
{
	int	exist;
	int	new_pid;

	srand((unsigned)time(0));

	for (int i = 0; i < num; i++)
	{
		do{
			exist = 0;
			new_pid = rand() % 10000;
			for (int j = 0; j < i; j++)
			{
				if (p_list[j].pid == new_pid)
				{
					exist = 1;
					break;
				}
			}
		} while(exist); // 중복되는 pid가 있을 경우 다시 만들기
		p_list[i].pid = new_pid;
	}
}

void	Config(void)
{
	int	min, min_idx, temp;

	job_queue.size = 0;
	ready_queue.size = 0;
	waiting_queue.size = 0;
	terminated_queue.size = 0;
	running_cpu = -1;
	running_io = -1;

	for (int i = 0; i < num; i++)
	{
		job_queue.queue[i] = i;
		p_list[i].CPU_rest = p_list[i].CPU_burst_time;
		p_list[i].IO_rest = p_list[i].IO_burst_time;
		p_list[i].rr_time = 0;
		p_list[i].terminated_time = 0;
		p_list[i].waiting_time = 0;
	}
	job_queue.size = num;
	
	for (int i = 0; i < num - 1; i++)
	{
		for (int j = i + 1; j < num; j++)
		{	
			if (p_list[job_queue.queue[i]].arrival_time > p_list[job_queue.queue[j]].arrival_time)
			{
				temp = job_queue.queue[i];
				job_queue.queue[i] = job_queue.queue[j];
				job_queue.queue[j] = temp;
			}
		}
	}
	printf("\n                      < JOB QUEUE >\n");
	print_job_queue();
}

void    arrive_process(int time)
{
    while (job_queue.size)
    {
        if (p_list[job_queue.queue[0]].arrival_time == time)
            queue_push(&ready_queue, queue_pop(&job_queue));
        else
            break ;
    }
}

int	terminate_check(int time)
{
    // CPU 실행했던 프로세스 끝나면 running_cpu 비우기
	if (running_cpu != -1 && 
		!p_list[running_cpu].CPU_rest && !p_list[running_cpu].IO_rest)
	{
		p_list[running_cpu].terminated_time = time;
		queue_push(&terminated_queue, running_cpu);
		p_list[running_cpu].terminated_time = time;
		running_cpu = -1;
	}

    // I/O 실행했던 프로세스 끝나면 running_io 비우기
	if (running_io != -1 && !p_list[running_io].IO_rest)
	{
		queue_push(&ready_queue, running_io);
		running_io = -1;
	}
	if (terminated_queue.size == num)
		return 1;
	return 0;
}

void	random_io_out(void)
{
	if (running_io >= 0 && p_list[running_io].IO_rest && p_list[running_io].CPU_rest > 1)
	{
		if (rand() % 2)
		{
			queue_push(&ready_queue, running_io);
			running_io = -1;
		}
	}
}

int	Do_IO(int real)
{
	srand((unsigned)time(0));
	if (real)
	{
		if (running_io >= 0)
			p_list[running_io].IO_rest -= 1;
		return running_io;
	}
	// 1. 아직 IO 없으면 waiting_queue에서 가져와서 Do_CPU(1), waiting_queue empty면 그냥 비움
	if (running_io == -1)
	{
		if (!waiting_queue.size)
			return Do_IO(1);
		running_io = queue_pop(&waiting_queue);
		return Do_IO(1);
	}
	// 2. IO 있으면 계속 실행. random하게 나가는 것은 따로 진행
	return Do_IO(1);
}
