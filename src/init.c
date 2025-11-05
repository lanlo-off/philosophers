/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:35:06 by llechert          #+#    #+#             */
/*   Updated: 2025/11/05 18:12:29 by llechert         ###   ########.fr       */
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
		clean_mutex_struct(philo, arg->nb_philo, 1);
		free(philo);
		free(forks);
	}
	else
	{
		philo->arg = arg;
		init_process(philo, forks, arg);
		clean_all(philo, forks, arg);
	}
}

bool	init_philo(t_philo *philo, t_args *arg)
{
	int	i;

	i = 0;
	while (i < arg->nb_philo)
	{
		philo[i].index = i;
		philo[i].last_meal_time = 0;
		if (pthread_mutex_init(&philo[i].last_meal_mutex, NULL) != 0)//proteger ce mutex init comme dans init forks avec destruction de ceux deja crees
		{
			clean_mutex_struct(philo, i, 1);
			return (false);
		}
		philo[i].meal_eaten = 0;
		if (pthread_mutex_init(&philo[i].nb_meal_mutex, NULL) != 0)//proteger ce mutex init comme dans init forks avec destruction de ceux deja crees
		{
			clean_mutex_struct(philo, i, 2);
			return (false);
		}
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
		if (pthread_mutex_init(&forks[i], NULL) != 0)//si erreur dans l'init destroy tous les mutex deja init
		{
			clean_mutex_tab(forks, i);
			return (false);
		}
		i++;
	}
	return (true);
}

void	init_process(t_philo *philo, pthread_mutex_t *forks, t_args *arg)
{
	t_monitor *monitor;

	monitor = malloc(sizeof(t_monitor));
	if (!monitor)
		return ;
	monitor->stop = false;
	monitor->abort = false;
	monitor->forks = &forks;
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
	else if (pthread_mutex_init(&monitor->abort_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&monitor->stop_mutex);
		pthread_mutex_destroy(&monitor->start_mutex);
		free(monitor);
		return ;
	}
	init_threads(monitor, philo, forks, arg);
	pthread_mutex_destroy(&monitor->stop_mutex);
	pthread_mutex_destroy(&monitor->start_mutex);
	pthread_mutex_destroy(&monitor->abort_mutex);
	free(monitor);
}

void	init_threads(t_monitor *monitor, t_philo *philo,
	pthread_mutex_t *forks, t_args *args)
{
	int	i;
	
	i = 0;
	monitor->start_mutex
	if (pthread_create(&monitor->m, NULL, &m_routine, NULL) != 0)//quel argument pour la routine, faut que le monitor puisse lire les data philo
		return ;
	while (i < args->nb_philo)
	{
		if (pthread_create(&philo[i].p, NULL, &p_routine, NULL) != 0)//quel argument pour la routine ? Faut qu'ils puissent lire les data du monitor et des forks
		{
			monitor->abort = 1;
			//destroy_threads(philo, i); en les lancant et avec le abort a 1 ca permet de les faire fermer
			//Puis on les join jusqu'a i = 0 pour que tout se ferme bien avant que le processus ne se termine
			return ;
		}
		i++;
	}
	
}
