/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzheng <yzheng@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:54:52 by yzheng            #+#    #+#             */
/*   Updated: 2024/11/15 11:17:40 by yzheng           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_pho(int ac, char **av, t_philo **pho)
{
	int	i;

	if (ac != 5 && ac != 6)
		return (0);
	if (!ft_isdigit(av))
		return (0);
	*pho = (t_philo *)malloc(sizeof(t_philo) * (ft_atoi(av[1]) + 1));
	if (!*pho)
		return (0);
	i = 0;
	while (i < ft_atoi(av[1]))
	{
		(*pho)[i].amount = ft_atoi(av[1]);
		(*pho)[i].time_to_die = ft_atoi(av[2]);
		(*pho)[i].time_to_eat = ft_atoi(av[3]);
		(*pho)[i].time_to_sleep = ft_atoi(av[4]);
		if (ac == 6)
			(*pho)[i].full_times = ft_atoi(av[5]);
		else
			(*pho)[i].full_times = 0;
		i++;
	}
	return (1);
}

int	create_threads(t_philo *pho, pthread_t *threads, pthread_mutex_t *forks,
		t_lock *locker)
{
	int	i;

	i = 0;
	while (i < pho[0].amount)
	{
		pho[i].id = i + 1;
		pho[i].self = locker->self;
		pho[i].print = locker->print;
		pho[i].left_fork = &forks[i];
		pho[i].right_fork = &forks[(i + 1) % pho[0].amount];
		gettimeofday(&pho[i].start_time, NULL);
		pho[i].last_meal_time = get_time(&pho[i]);
		pho[i].is_dead = 0;
		pho[i].eaten = 0;
		if (pthread_create(&threads[i], NULL, &philo_routine, &pho[i]) == -1)
		{
			free_all(pho, forks, threads);
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_thread(t_philo *pho, pthread_mutex_t *forks, t_lock *locker)
{
	pthread_t	*threads;
	int			i;

	i = 0;
	threads = (pthread_t *)malloc(pho[0].amount * sizeof(pthread_t));
	if (!threads)
	{
		free_lock(pho, forks, locker->self, locker->print);
		return (0);
	}
	if (!create_threads(pho, threads, forks, locker))
		return (0);
	listener(pho);
	while (i < pho[0].amount)
		pthread_join(threads[i++], NULL);
	free(locker->self);
	free(locker->print);
	free(locker);
	free_all(pho, forks, threads);
	return (1);
}

int	init_lock(t_philo *pho, pthread_mutex_t **forks, pthread_mutex_t **self,
		pthread_mutex_t **print)
{
	int	i;

	i = 0;
	*forks = (pthread_mutex_t *)malloc(pho[0].amount * sizeof(pthread_mutex_t));
	*self = (pthread_mutex_t *)malloc(pho[0].amount * sizeof(pthread_mutex_t));
	*print = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (!check_malloc(pho, forks, self, print))
		return (0);
	while (i < pho[0].amount)
	{
		if (pthread_mutex_init(&(*forks)[i], NULL) != 0)
			return (free_lock(pho, *forks, *self, *print));
		if (pthread_mutex_init(&(*self)[i], NULL) != 0)
			return (free_lock(pho, *forks, *self, *print));
		i++;
	}
	if (pthread_mutex_init((*print), NULL) != 0)
		return (free_lock(pho, *forks, *self, *print));
	return (1);
}

int	main(int ac, char **av)
{
	t_philo			*pho;
	pthread_mutex_t	*forks;
	t_lock			*locker;

	locker = (t_lock *)malloc(sizeof(t_lock));
	if (!locker)
		return (1);
	if (!init_pho(ac, av, &pho))
	{
		printf("Check your arguments\n");
		return (1);
	}
	if (!init_lock(pho, &forks, &locker->self, &locker->print))
	{
		free(locker);
		return (1);
	}
	return (init_thread(pho, forks, locker));
}
