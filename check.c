/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzheng <yzheng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:41:36 by yzheng            #+#    #+#             */
/*   Updated: 2024/11/14 19:00:33 by yzheng           ###   ########.fr       */
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
	if (philo->is_dead || (philo->eaten == philo->full_times && philo->eaten))
	{
		pthread_mutex_unlock(philo->self);
		return (0);
	}
	pthread_mutex_unlock(philo->self);
	if (philo->full_times)
		philo->eaten++;
	if (philo->eaten == philo->full_times && philo->eaten)
		return (0);
	print_message(philo, 2);
	precise_usleep(philo->time_to_sleep * 1000);
	return (1);
}

void	check_listener(t_philo *pho, int i)
{
	long	timediff;

	timediff = get_time(&pho[i]) - pho[i].last_meal_time;
	if (timediff >= pho[i].time_to_die)
	{
		pho[i].is_dead = 1;
		pthread_mutex_lock(pho[i].print);
		printf("%ld %d died\n", get_time(&pho[i]), pho[i].id);
		pthread_mutex_unlock(pho[i].print);
	}
	pthread_mutex_unlock(pho[i].self);
}

void	listener(t_philo *pho)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (1)
	{
		pthread_mutex_lock(pho[i].self);
		if (all_done(pho, i))
			break ;
		check_listener(pho, i);
		if (pho[i].is_dead)
		{
			while (j < pho[0].amount)
			{
				pthread_mutex_lock(pho[j].self);
				pho[j].is_dead = 1;
				pthread_mutex_unlock(pho[j++].self);
			}
			break ;
		}
		i = (i + 1) % pho[0].amount;
	}
}
