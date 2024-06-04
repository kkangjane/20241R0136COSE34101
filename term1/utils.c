#include "scheduler.h"

void print_chart(int chart[], int time)
{
	for (int i = 0; i < time; i++)
	{
		if (chart[i] < 0)
			printf("|  |");
		else
			printf("|P%d|", chart[i]);
	}
	printf("\n");
	for (int i = 0; i < time; i++)
	{
		printf("%-2d  ", i);
	}
	printf("\n\n");
}

void	sum_time(int *cpu, int *io)
{
	*cpu = 0;
	*io = 0;

	for (int i = 0; i < num; i++)
	{
		*cpu += p_list[i].CPU_burst_time;
		*io += p_list[i].IO_burst_time;
	}
}

void	print_result(int cpu_chart[], int io_chart[], int cpu_check, int io_check, int time)
{
	int	turnarround_time = 0;
	int waiting_time = 0;

	printf("CPU Gantt Chart\n");
	print_chart(cpu_chart, time);
	printf("I/O Gantt Chart\n");
	print_chart(io_chart, time);
	
	if (cpu_check != 0)
		printf("CPU operation scheduling failed\n");
	if (io_check != 0)
		printf("I/O operation scheduling failed\n");
	
	for (int i = 0; i < num; i++)
	{
		turnarround_time += p_list[i].terminated_time;
		turnarround_time -= p_list[i].arrival_time;

		waiting_time += p_list[i].terminated_time;
		waiting_time -= p_list[i].arrival_time;
		waiting_time -= p_list[i].CPU_burst_time;
	}
	printf("Average Turnarround time: %d\n", turnarround_time / num);
	printf("Average Waiting time: %d\n\n\n", waiting_time / num);
}

void	print_processes(void)
{
	// idx | pid | CPU burst time | I/O burst time | arrival time | priority
	printf("\n                 < Information of processes >\n");
	printf("---------------------------------------------------------------\n");
	printf("| idx |  pid  | priority | CPU time | I/O time | arrival time |\n");
	printf("---------------------------------------------------------------\n");
	
	for (int i = 0; i < num; i++)
		printf("|  %d  |  %4d |   %3d    |    %2d    |    %2d    |      %2d      |\n", i, p_list[i].pid, p_list[i].priority, p_list[i].CPU_burst_time, p_list[i].IO_burst_time, p_list[i].arrival_time);
	
	printf("---------------------------------------------------------------\n");
}


void	queue_push(t_queue *queue, int process_idx)
{
	int	top = queue->size;
	queue->queue[top] = process_idx;
	queue->size += 1;
}

int	queue_pop(t_queue *queue)
{
	int temp = queue->queue[0];
	for (int i = 0; i < queue->size - 1; i++)
		queue->queue[i] = queue->queue[i + 1];
	queue->size -= 1;
	return temp;
}

int	queue_pop_priority(int pop)
{
	int cpu = ready_queue.queue[0];
	int priority = p_list[cpu].priority;

	for (int i = 0; i < ready_queue.size; i++)
	{
		if (priority > p_list[ready_queue.queue[i]].priority)
		{
			cpu = ready_queue.queue[i];
			priority = p_list[cpu].priority;
		}
	}

	if (pop == 0)
		return cpu;
	int j = 0;
	for (int i = 0; i < ready_queue.size; i++)
		if (ready_queue.queue[i] != cpu)
			ready_queue.queue[j++] = ready_queue.queue[i];
	ready_queue.size -= 1;
	return cpu;
}

int	queue_pop_SJF(int pop)
{
	int cpu = ready_queue.queue[0];
	int time = p_list[cpu].CPU_rest;

	for (int i = 0; i < ready_queue.size; i++)
	{
		if (time > p_list[ready_queue.queue[i]].CPU_rest)
		{
			cpu = ready_queue.queue[i];
			time = p_list[cpu].CPU_rest;
		}
	}
	if (pop == 0)
		return cpu;

	int j = 0;
	for (int i = 0; i < ready_queue.size; i++)
		if (ready_queue.queue[i] != cpu)
			ready_queue.queue[j++] = ready_queue.queue[i];
	ready_queue.size -= 1;
	return cpu;
}

void	print_job_queue(void)
{
	int i;

	printf("---------------------------------------------------------------\n");
	printf("| idx |  pid  | priority | CPU time | I/O time | arrival time |\n");
	printf("---------------------------------------------------------------\n");
	for (int j = 0; j < num; j++)
	{
		i = job_queue.queue[j];
		printf("|  %d  |  %4d |   %3d    |    %2d    |    %2d    |      %2d      |\n", i, p_list[i].pid, p_list[i].priority, p_list[i].CPU_burst_time, p_list[i].IO_burst_time, p_list[i].arrival_time);
	}
	printf("---------------------------------------------------------------\n");
}