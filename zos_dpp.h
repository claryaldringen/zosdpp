/*
 * File:   zos_dpp.h
 * Author: clary
 *
 * Created on 29. listopad 2013, 6:46
 */

#ifndef ZOS_DPP_H
#define	ZOS_DPP_H

struct thread_params
{
	int id;
};

void get_rice_to_plates();

void sit_philosophers_around_table();

int get_left_fork(int);

int get_right_fork(int);

void eat(int);

void put_down_forks(int, int*, int*);

int allowed_from_waiter(int);

void show_status();

void show_forks();

void philosophize(int);

void parse_args(int, char**);

#endif	/* ZOS_DPP_H */

