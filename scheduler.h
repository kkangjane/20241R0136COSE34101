#ifndef SHCEDULHER_H
# define SHCEDULHER_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>

# define MAX_PNUM 	10

# define FCFS		1
# define SJF		2
# define P_SJF		3
# define PRIORITY	4
# define P_PRIORITY	5
# define RR			6

# define WAITING	1
# define READY		2
# define JOB		3
# define TERMINATED	4
# define RR_TIME	2

typedef struct s_process
{
    int		pid;
    int     CPU_burst_time;
    int     IO_burst_time;
	int		CPU_rest;
	int		IO_rest;
    int     arrival_time;
    int     priority;
	int		rr_time;
	int		terminated_time;
} t_process;

typedef struct s_queue
{
	int	size;
	int	queue[MAX_PNUM];
} t_queue;


t_process   p_list[MAX_PNUM];
t_queue		ready_queue;
t_queue		waiting_queue;
t_queue		job_queue;
t_queue		terminated_queue;

int			running_cpu;
int			running_io;
int         num; // process 개수

// schedule.c
void	FCFS_scheduling(void);
void	SJF_scheduling(void);
void	Preemptive_SJF_scheduling(void);
void	Priority_scheduling(void);
void	Preemptive_Priority_scheduling(void);
void	RR_scheduling(void);

// process.c
void	Create_Process(void);
void	make_pid(void);
void	Config(void);
void    arrive_process(int time);
int	    terminate_check(int time);
void	random_io_out(void);
int		Do_IO(int real);


// utils.c
void	print_chart(int chart[], int time);
void	print_process_state(void);
void	sum_time(int *cpu, int *io);
void	print_result(int cpu_chart[], int io_chart[], int cpu_check, int io_check, int time);
void	print_processes(void);
void	queue_push(t_queue *queue, int process_idx);
int		queue_pop(t_queue *queue);
int		queue_pop_SJF(int pop);
int		queue_pop_priority(int pop);

#endif