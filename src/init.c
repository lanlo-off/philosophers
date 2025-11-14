/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:06:06 by llechert          #+#    #+#             */
/*   Updated: 2025/11/14 16:47:31 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	init_data(t_data *data)
{
	if (pthread_mutex_init(&data->start_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->start_mutex);
		return (false);
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->start_mutex);
		pthread_mutex_destroy(&data->stop_mutex);
		return (false);
	}
	data->stop = false;
	return (true);
}

bool	init_forks(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (false);
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			clean_mutex_tab(data->forks, i);
			free(data->forks);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	init_philo(t_data *data)
{
	int	i;

	i = 0;
	data->philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philo)
		return (false);
	while (i < data->nb_philo)
	{
		fill_philo(data, i);
		if (pthread_mutex_init(&data->philo[i].last_meal_mutex, NULL) != 0)
		{
			clean_mutex_philo(data->philo, i, 1);
			free(data->philo);
			return (false);
		}
		if (pthread_mutex_init(&data->philo[i].nb_meal_mutex, NULL) != 0)
		{
			clean_mutex_philo(data->philo, i, 2);
			free(data->philo);
			return (false);
		}
		i++;
	}
	return (true);
}

void	fill_philo(t_data *data, int i)
{
	data->philo[i].index = i + 1;
	data->philo[i].r_fork = i;
	if (i == data->nb_philo - 1)
		data->philo[i].l_fork = 0;
	else
		data->philo[i].l_fork = i + 1;
	data->philo[i].last_meal_time = 0;
	data->philo[i].meal_eaten = 0;
	data->philo[i].data = data;
}

void	init_threads(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_time_ms();
	pthread_mutex_lock(&data->start_mutex);
	if (pthread_create(&data->m, NULL, &m_rout, data) != 0)
	{
		pthread_mutex_unlock(&data->start_mutex);
		clean_and_exit(data, 3);
	}
	while (i < data->nb_philo)
	{
		if (pthread_create(&data->philo[i].p, NULL, &p_rout, &data->philo[i]))
		{
			data->stop = true;
			pthread_mutex_unlock(&data->start_mutex);
			pthread_join(data->m, NULL);
			while (--i >= 0)
				pthread_join(data->philo[i].p, NULL);
			clean_and_exit(data, 3);
		}
		i++;
	}
	pthread_mutex_unlock(&data->start_mutex);
	clean_and_exit(data, 4);
}
