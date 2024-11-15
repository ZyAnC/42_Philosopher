/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzheng <yzheng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 20:23:59 by yzheng            #+#    #+#             */
/*   Updated: 2024/11/15 12:07:34 by yzheng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <semaphore.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct lockers
{
	pthread_mutex_t	*self;
	pthread_mutex_t	*print;
}					t_lock;

typedef struct philo
{
	int				id;
	int				amount;
	int				eaten;
	long			*ptime;
	long			*ntime;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				full_times;
	long			last_meal_time;
	int				is_dead;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*self;
	pthread_mutex_t	*print;
	struct timeval	start_time;
}					t_philo;

int					ft_atoi(const char *str);
int					ft_isdigit(char **av);
long				get_microseconds(struct timeval start, struct timeval end);
void				precise_usleep(long usec);
long				get_time(t_philo *philo);
int					philo_check(t_philo *philo);
void				free_all(t_philo *pho, pthread_mutex_t *forks,
						pthread_t *threads);
void				*philo_routine(void *arg);
void				free_pho_fork(t_philo *pho, pthread_mutex_t *fork);
void				listener(t_philo *pho);
void				print_message(t_philo *philo, int i);
int					check_malloc(t_philo *pho, pthread_mutex_t **forks,
						pthread_mutex_t **self, pthread_mutex_t **print);
int					all_done(t_philo *pho);
int					free_lock(t_philo *pho, pthread_mutex_t *fork,
						pthread_mutex_t *self, pthread_mutex_t *print);
int					after_full(t_philo *philo);
int					get_dead(t_philo *pho, int i, int model);
int					philo_even(t_philo *philo);
int					philo_odd(t_philo *philo);
#endif
