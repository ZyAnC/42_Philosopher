/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzheng <yzheng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:19:02 by yzheng            #+#    #+#             */
/*   Updated: 2024/11/14 18:57:30 by yzheng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_message(t_philo *philo, int i)
{
	pthread_mutex_lock(philo->print);
	if (i == 0)
	{
		printf("%ld %d has taken a fork\n", get_time(philo), philo->id);
		printf("%ld %d has taken a fork\n", get_time(philo), philo->id);
		printf("%ld %d is eating\n", get_time(philo), philo->id);
	}
	else if (i == 1)
		printf("%ld %d is thinking\n", get_time(philo), philo->id);
	else if (i == 2)
		printf("%ld %d is sleeping\n", get_time(philo), philo->id);
	else if (i == 3)
		printf("%ld %d died\n", get_time(philo), philo->id);
	pthread_mutex_unlock(philo->print);
}

int	check_malloc(t_philo *pho, pthread_mutex_t **forks, pthread_mutex_t **self,
		pthread_mutex_t **print)
{
	if (!*forks)
	{
		free(pho);
		return (0);
	}
	if (!self)
	{
		free_pho_fork(pho, *forks);
		return (0);
	}
	if (!print)
	{
		free_pho_fork(pho, *forks);
		free(*self);
		return (0);
	}
	return (1);
}

int	all_done(t_philo *pho, int j)
{
	int	i;

	i = 0;
	while (i < pho[0].amount)
	{
		if (pho[i].eaten != pho[i].full_times || !pho[i].full_times)
			return (0);
		i++;
	}
	pthread_mutex_unlock(pho[j].self);
	return (1);
}

int	free_lock(t_philo *pho, pthread_mutex_t *fork, pthread_mutex_t *self,
		pthread_mutex_t *print)
{
	free(pho);
	free(fork);
	free(self);
	free(print);
	return (0);
}
