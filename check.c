/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzheng <yzheng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:41:36 by yzheng            #+#    #+#             */
/*   Updated: 2024/11/15 15:23:13 by yzheng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	digit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isdigit(char **av)
{
	int	i;
	int	j;

	j = 1;
	while (av[j])
	{
		i = 0;
		if (ft_atoi(av[j]) == 0 || ((strlen(av[j]) >= 10)
				&& (ft_atoi(av[j]) == 0 || ft_atoi(av[j]) == -1)))
			return (0);
		while (av[j][i])
		{
			if (!digit(av[j][i]))
				return (0);
			i++;
		}
		j++;
	}
	return (1);
}

int	philo_check(t_philo *philo)
{
	pthread_mutex_lock(philo->self);
	if ((philo->eaten == philo->full_times && philo->eaten))
	{
		pthread_mutex_unlock(philo->self);
		return (after_full(philo));
	}
	if (philo->is_dead)
	{
		pthread_mutex_unlock(philo->self);
		return (0);
	}
	pthread_mutex_unlock(philo->self);
	if (philo->full_times)
		philo->eaten++;
	if (philo->eaten == philo->full_times && philo->eaten)
		return (after_full(philo));
	if (get_dead(philo, 0, 2))
		return (0);
	print_message(philo,2);
	precise_usleep(philo->time_to_sleep * 1000);
	return (1);
}

void	check_listener(t_philo *pho, int i)
{
	long	timediff;

	pthread_mutex_lock(pho[i].self);
	timediff = get_time(&pho[i]) - pho[i].last_meal_time;
	pthread_mutex_unlock(pho[i].self);
	if (timediff >= pho[i].time_to_die)
	{
		pthread_mutex_lock(pho[i].self);
		pho[i].is_dead = 1;
		pthread_mutex_unlock(pho[i].self);
		pthread_mutex_lock(pho[i].print);
		printf("%ld %d died\n", get_time(&pho[i]), pho[i].id);
		pthread_mutex_unlock(pho[i].print);
	}
}

void	listener(t_philo *pho)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (1)
	{
		if (all_done(pho))
			break ;
		check_listener(pho, i);
		if (get_dead(pho, i, 0))
		{
			while (j < pho[0].amount)
			{
				if (j != i)
					get_dead(pho, j, 1);
				j++;
			}
			break ;
		}
		i = (i + 1) % pho[0].amount;
	}
}
