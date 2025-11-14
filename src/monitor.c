/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:28:31 by llechert          #+#    #+#             */
/*   Updated: 2025/11/14 16:44:23 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief commence par verifier si le philo a deja mange assez
 * et s'arrete si tous les philos sont pleins
 * Puis verifie s'il est mort et s'arrete si oui
 * 
 * @param arg
 */
void	*m_rout(void *arg)
{
	t_data	*data;
	int		i;
	int		goal_count;

	data = (t_data *)arg;
	goal_count = 0;
	while (!check_stop(data))
	{
		i = 0;
		goal_count = 0;
		while (i < data->nb_philo)
		{
			if (philo_full(data, &data->philo[i]))
				goal_count++;
			if (all_philo_full(data, goal_count))
				break ;
			if (check_death(data, &data->philo[i]))
				break ;
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}

bool	philo_full(t_data *data, t_philo *philo)
{
	bool	val;

	pthread_mutex_lock(&philo->nb_meal_mutex);
	if (data->goal > 0 && philo->meal_eaten >= data->goal)
		val = true;
	else
		val = false;
	pthread_mutex_unlock(&philo->nb_meal_mutex);
	return (val);
}

bool	all_philo_full(t_data *data, int goal_count)
{
	if (data->nb_philo == goal_count)
	{
		set_stop(data, true);
		return (true);
	}
	return (false);
}

void	set_stop(t_data *data, bool val)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->stop = val;
	pthread_mutex_unlock(&data->stop_mutex);
}

bool	check_death(t_data *data, t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_mutex);
	if (time_passed(data) - philo->last_meal_time >= data->tt_die)
	{
		pthread_mutex_unlock(&philo->last_meal_mutex);
		print_info(data, philo->index, "died");
		set_stop(data, true);
		return (true);
	}
	pthread_mutex_unlock(&philo->last_meal_mutex);
	return (false);
}
