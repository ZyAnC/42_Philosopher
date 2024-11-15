/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzheng <yzheng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:04:28 by yzheng            #+#    #+#             */
/*   Updated: 2024/11/15 16:28:53 by yzheng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_dead(t_philo *pho, int i, int model)
{
	int	dead;

	dead = 0;
	if (model == 0)
	{
		pthread_mutex_lock(pho[i].self);
		dead = pho[i].is_dead;
		pthread_mutex_unlock(pho[i].self);
	}
	else if (model == 1)
	{
		pthread_mutex_lock(pho[i].self);
		pho[i].is_dead = 1;
		pthread_mutex_unlock(pho[i].self);
	}
	else
	{
		pthread_mutex_lock(pho->self);
		dead = pho->is_dead;
		pthread_mutex_unlock(pho->self);
	}
	return (dead);
}

static int	philo_eat(t_philo *philo)
{
	if ((philo->eaten == philo->full_times && philo->eaten))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (after_full(philo));
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
	precise_usleep(philo->time_to_eat * 500);
	if ((philo->eaten == philo->full_times && philo->eaten))
		return (after_full(philo));
	pthread_mutex_lock(philo->self);
	if (philo->is_dead)
	{
		pthread_mutex_unlock(philo->self);
		return (0);
	}

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

int	all_done(t_philo *pho)
{
	int	i;
	int	eaten;
	int	full;

	i = 0;
	while (i < pho[0].amount)
	{
		pthread_mutex_lock(pho[i].self);
		eaten = pho[i].eaten;
		full = pho[i].full_times;
		pthread_mutex_unlock(pho[i].self);
		if (eaten != full || !full)
		{
			return (0);
		}
		i++;
	}
	return (1);
}
