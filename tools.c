/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzheng <yzheng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 18:39:27 by yzheng            #+#    #+#             */
/*   Updated: 2024/11/14 16:07:34 by yzheng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-')
	{
		sign *= -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while ((*str >= '0' && *str <= '9'))
	{
		result = result * 10 + (*str - '0');
		if (result < 0 && sign > 0)
			return (-1);
		if (result < 0 && sign < 0)
			return (0);
		str++;
	}
	return ((int)(result * sign));
}

long	get_microseconds(struct timeval start, struct timeval end)
{
	long	time;

	time = end.tv_sec - start.tv_sec * 1000000;
	return (time + (end.tv_usec - start.tv_usec));
}

void	precise_usleep(long usec)
{
	long			elapsed;
	struct timeval	start;
	struct timeval	current;

	gettimeofday(&start, NULL);
	while (1)
	{
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000000 + (current.tv_usec
				- start.tv_usec);
		if (elapsed >= usec)
			break ;
		usleep(1000);
	}
}

long	get_time(t_philo *philo)
{
	struct timeval	current_time;
	long			elapsed_sec;
	long			elapsed_usec;

	gettimeofday(&current_time, NULL);
	elapsed_sec = current_time.tv_sec - philo->start_time.tv_sec;
	elapsed_usec = current_time.tv_usec - philo->start_time.tv_usec;
	return ((elapsed_sec * 1000) + (elapsed_usec / 1000));
}

void	free_all(t_philo *pho, pthread_mutex_t *forks, pthread_t *threads)
{
	free(pho);
	free(forks);
	free(threads);
}
