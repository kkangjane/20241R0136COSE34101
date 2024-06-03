#include "scheduler.h"

int main(void)
{
	int	type;
	char s;
   
	printf("How many process do you want?\n> ");
	scanf("%d", &num);
	if (num > MAX_PNUM || num <= 0)
	{
		printf("I can't. Let's make %d processes\n", MAX_PNUM);
		num = MAX_PNUM;
	}

    Create_Process();

	while(1)
	{
		printf("Which type of scheduling do you want?\n");
		printf("1.FCFS 2.SJF 3.preemptive SJF 4.Priority 5.preemptive Priority 6.RR\n> ");
		scanf("%d", &type);
		print_processes();
		if (type == 1)
		{
			printf("\n\n-----------------------FCFS scheduling-----------------------\n");
			Config();
			FCFS_scheduling();
		}
		else if (type == 2)
		{
			printf("-----------------------SJF scheduling------------------------\n");
			Config();
			SJF_scheduling();
		}
		else if (type == 3)
		{
			printf("------------------preemptive SJF scheduling------------------\n");
			Config();
			Preemptive_SJF_scheduling();
		}
		else if (type == 4)
		{
			printf("---------------------Priority scheduling---------------------\n");
			Config();
			Priority_scheduling();
		}
		else if (type == 5)
		{
			printf("----------------preemptive Priority scheduling---------------\n");
			Config();
			Preemptive_Priority_scheduling();
		}
		else if (type == 6)
		{
			printf("-------------------------RR scheduling-----------------------\n");
			Config();
			RR_scheduling();
		}
			else
			{
				printf("finished\n");
				return 0;
			}
	}
	return 0;
}
