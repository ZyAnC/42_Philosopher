/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_running.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzheng <yzheng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:54:40 by yzheng            #+#    #+#             */
/*   Updated: 2024/11/14 18:57:19 by yzheng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	free_pho_fork(t_philo *pho, pthread_mutex_t *fork)
{
	free(pho);
	free(fork);
}

int	philo_eat(t_philo *philo)
{
	if ((philo->eaten == philo->full_times && philo->eaten))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	pthread_mutex_lock(philo->self);
	if (philo->is_dead)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->self);
		return (0);
	}
	pthread_mutex_unlock(philo->self);
	print_message(philo, 0);
	pthread_mutex_lock(philo->self);
	philo->last_meal_time = get_time(philo);
	pthread_mutex_unlock(philo->self);
	precise_usleep(philo->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (1);
}

int	philo_even(t_philo *philo)
{
	if ((philo->eaten == philo->full_times && philo->eaten))
		return (0);
	pthread_mutex_lock(philo->self);
	if (philo->is_dead)
	{
		pthread_mutex_unlock(philo->self);
		return (0);
	}
	if ((get_time(philo) - philo->last_meal_time) < (philo->time_to_die
			- philo->time_to_eat - 1))
		precise_usleep(500);
	pthread_mutex_unlock(philo->self);
	if (pthread_mutex_lock(philo->right_fork) == 0)
	{
		if (pthread_mutex_lock(philo->left_fork) == 0)
		{
			if (!philo_eat(philo))
				return (0);
			if (!philo_check(philo))
				return (0);
		}
	}
	return (1);
}

int	philo_odd(t_philo *philo)
{
	if ((philo->eaten == philo->full_times && philo->eaten))
		return (0);
	pthread_mutex_lock(philo->self);
	if (philo->is_dead || (philo->eaten == philo->full_times && philo->eaten))
	{
		pthread_mutex_unlock(philo->self);
		return (0);
	}
	if ((get_time(philo) - philo->last_meal_time) < (philo->time_to_die
			- philo->time_to_eat - 1))
		precise_usleep(500);
	pthread_mutex_unlock(philo->self);
	if (pthread_mutex_lock(philo->left_fork) == 0)
	{
		if (pthread_mutex_lock(philo->right_fork) == 0)
		{
			if (!philo_eat(philo))
				return (0);
			if (!philo_check(philo))
				return (0);
		}
	}
	return (1);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->amount <= 1)
		print_message(philo, 1);
	while (!philo->is_dead && philo->amount > 1)
	{
		if (philo->amount > 1)
		{
			if (philo->id % 2 == 1)
			{
				if (!philo_even(philo))
					break ;
			}
			else
			{
				if (!philo_odd(philo))
					break ;
			}
		}
	}
	return (NULL);
}
