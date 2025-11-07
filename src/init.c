/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:35:06 by llechert          #+#    #+#             */
/*   Updated: 2025/11/07 17:26:15 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	init_all(t_args *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*forks;

	philo = malloc(sizeof(t_philo) * arg->nb_philo);
	if (!philo)
		return ;
	if (!init_philo(philo, arg))
	{
		free(philo);
		return ;
	}
	forks = malloc(sizeof(pthread_mutex_t) * arg->nb_philo);
	if (!forks)
		free(philo);
	else if (!init_forks(forks, arg))
	{
		clean_mutex_philo(philo, arg->nb_philo, 1);
		free(philo);
		free(forks);
	}
	else
	{
		init_monitor(philo, forks, arg);
		clean_all(philo, forks, arg);
	}
}

bool	init_philo(t_philo *philo, t_args *arg)
{
	int	i;

	i = 0;
	while (i < arg->nb_philo)
	{
		philo[i].index = i + 1;
		philo[i].last_meal_time = 0;
		philo[i].arg = arg;
		if (pthread_mutex_init(&philo[i].last_meal_mutex, NULL) != 0)
		{
			clean_mutex_philo(philo, i, 1);
			return (false);
		}
		philo[i].meal_eaten = 0;
		if (pthread_mutex_init(&philo[i].nb_meal_mutex, NULL) != 0)
		{
			clean_mutex_philo(philo, i, 2);
			return (false);
		}
		def_left_n_right(&philo[i]);
		printf("philo %d : lfork : %d | rfork %d\n", philo[i].index, philo[i].l_fork, philo[i].r_fork);
		i++;
	}
	return (true);
}

bool	init_forks(pthread_mutex_t *forks, t_args *arg)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < arg->nb_philo)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)//si erreur dans l'init : destroy tous les mutex deja init
		{
			clean_mutex_tab(forks, i);
			return (false);
		}
		i++;
	}
	return (true);
}

void	init_monitor(t_philo *philo, pthread_mutex_t *forks, t_args *arg)
{
	t_monitor *monitor;

	monitor = malloc(sizeof(t_monitor));
	if (!monitor)
		return ;
	monitor->stop = false;
	monitor->arg = arg;
	monitor->forks = forks;
	assign_monitor(monitor, philo);
	if (pthread_mutex_init(&monitor->stop_mutex, NULL) != 0)
	{
		free(monitor);
		return ;
	}
	else if (pthread_mutex_init(&monitor->start_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&monitor->stop_mutex);
		free(monitor);
		return ;
	}
	init_threads(monitor, philo);
	pthread_mutex_destroy(&monitor->stop_mutex);
	pthread_mutex_destroy(&monitor->start_mutex);
	free(monitor);
}

void	init_threads(t_monitor *monitor, t_philo *philo)
{
	int	i;
	
	i = 0;
	pthread_mutex_lock(&monitor->start_mutex);
	if (pthread_create(&monitor->m, NULL, &m_routine, philo) != 0)
	{
		pthread_mutex_unlock(&monitor->start_mutex);
		return ;
	}
	monitor->start_time = get_time_ms();
	while (i < monitor->arg->nb_philo)
	{
		if (pthread_create(&philo[i].p, NULL, &p_routine, &philo[i]) != 0)//quel argument pour la routine ? Faut qu'ils puissent lire les data du monitor et des forks
		{
			monitor->stop = true;
			pthread_mutex_unlock(&monitor->start_mutex);
			pthread_join(monitor->m, NULL);
			while (--i >= 0)
                pthread_join(philo[i].p, NULL);
            return ;
		}
		i++;
	}
	pthread_mutex_unlock(&monitor->start_mutex);//lance les threads
	join_threads(monitor, philo);
}
