/*
 * File:   main.c
 * Author: clary
 *
 * Created on 29. listopad 2013, 6:15
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "zos_dpp.h"


int plates[10];

int forks[10];

int philosophers_count = 5;

pthread_mutex_t mut_var;

void *philosopher_task(void *param)
{
	int *par = (int *)param;
	int id = *par;
	int left_fork = 0;
	int right_fork = 0;
	while(plates[id] > 0)
	{
		pthread_mutex_lock(&mut_var);
		if(!left_fork && allowed_from_waiter(id))left_fork = get_left_fork(id);
		if(left_fork)right_fork = get_right_fork(id);
		pthread_mutex_unlock(&mut_var);
		if(right_fork)eat(id);
		put_down_forks(id, &left_fork, &right_fork);
		philosophize(id);
	}
	printf("Filozof %d odchází od stolu.\n", id);
  return(NULL);
}

int main(int argc, char** argv)
{
	parse_args(argc, argv);
	get_rice_to_plates();
	sit_philosophers_around_table();
	return (EXIT_SUCCESS);
}

void get_rice_to_plates()
{
	for(int i = 0; i < philosophers_count; i++)
	{
		plates[i] = 10000;
		forks[i] = 1;
	}
}

void sit_philosophers_around_table()
{
	pthread_t thread_id[10];
	int ids[10];
	for(int i = 0; i < philosophers_count; i++)
	{
		ids[i] = i;
		pthread_create(&thread_id[i], NULL, &philosopher_task, &ids[i]);

	}
	for(int i = 0; i < philosophers_count; i++)
	{
		pthread_join(thread_id[i], NULL);
	}
	printf("\n");
}

int get_left_fork(int id)
{
	int fork_id = id;
	printf("Filozof %d se pokouší vzít levou vidličku.\n", id);
	if(forks[fork_id] == 1)
	{
		forks[fork_id] = 0;
		printf("Filozof %d vzal levou vidličku.\n", id);
		return 1;
	}
	printf("Filozof %d nevzal levou vidličku.\n", id);
	return 0;
}

int get_right_fork(int id)
{
	int fork_id = id + 1;
	printf("Filozof %d se pokouší vzít pravou vidličku.\n", id);
	if(fork_id >= philosophers_count)fork_id = 0;
	if(forks[fork_id] == 1)
	{
		forks[fork_id] = 0;
		printf("Filozof %d vzal pravou vidličku.\n", id);
		return 1;
	}
	printf("Filozof %d nevzal pravou vidličku.\n", id);
	return 0;
}

void eat(int id)
{
	printf("Filozof %d právě jí.\n", id);
	for(int i = 0;i < 1000;i++)
	{
		plates[id]--;
		if(!(plates[id]%1000))printf("Filozof %d má v misce %d zrnek rýže.\n", id, plates[id]);
	}
}

void put_down_forks(int id, int* left_fork, int* right_fork)
{
	pthread_mutex_lock(&mut_var);
	int fork_id = id;
	if(*left_fork)printf("Filozof %d pokládá levou vidličku.\n", id);
	if(*right_fork)printf("Filozof %d pokládá pravou vidličku.\n", id);
	forks[fork_id] += *left_fork;
	fork_id++;
	if(fork_id >= philosophers_count)fork_id = 0;
	forks[fork_id] += *right_fork;
	if(*left_fork)printf("Filozof %d položil levou vidličku.\n", id);
	if(*right_fork)printf("Filozof %d položil pravou vidličku.\n", id);
	*left_fork = 0;
	*right_fork = 0;
	pthread_mutex_unlock(&mut_var);
}

void show_forks()
{
	for(int i = 0; i < philosophers_count; i++)
	{
		printf(" %d ", forks[i]);
	}
	printf("\n");
}

int allowed_from_waiter(int id)
{
	int fork_id = id;
	int left_fork_id  = id-1;
	int right_fork_id = id+1;
	if(fork_id == 0)left_fork_id = philosophers_count - 1;
	if(right_fork_id == philosophers_count)right_fork_id = 0;
	if(forks[left_fork_id] && forks[right_fork_id] && forks[fork_id])return 1;
	return 0;
}

void philosophize(int id)
{
	int prime;
	printf("Filozof %d začíná filozofovat.\n", id);
	for(int number = 10000000; number < 10000100; number++)
	{
		prime = 1;
		for(int divisor = 2; divisor < number; divisor++)
		{
			if((number%divisor) == 0)
			{
				prime = 0;
				break;
			}
		}
		if(prime)printf("Filozof %d vyfilozofoval, že %d je prvočíslo.\n", id, number);

	}
	printf("Filozof %d přestává filozofovat.\n", id);
}

void parse_args(int argc, char** argv)
{
	if(argc == 2)
	{
		philosophers_count = atoi(argv[1]);
	}
}