/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_running.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzheng <yzheng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:54:40 by yzheng            #+#    #+#             */
/*   Updated: 2024/11/15 15:35:22 by yzheng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	after_full(t_philo *philo)
{
	print_message(philo, 1);
	return (0);
}

static int	philo_running(t_philo *philo)
{
	if (philo->id % 2 == 1)
	{
		if (get_dead(philo, 0, 2))
			return (0);
		print_message(philo, 1);
		if (!philo_even(philo))
			return (0);
	}
	else
	{
		if (get_dead(philo, 0, 2))
			return (0);
		print_message(philo, 1);
		if (!philo_odd(philo))
			return (0);
	}
	return (1);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->amount <= 1)
		print_message(philo, 1);
	while (!get_dead(philo, 0, 2) && philo->amount > 1)
	{
		if (philo->amount > 1)
			if (!philo_running(philo))
				break ;
	}
	return (NULL);
}
