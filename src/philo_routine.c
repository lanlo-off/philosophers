/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:12:56 by llechert          #+#    #+#             */
/*   Updated: 2025/11/14 16:41:10 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*p_rout(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->nb_philo == 1)
	{
		alone_routine(philo);
		return (NULL);
	}
	if (philo->index % 2 == 1)
		usleep(1000);
	while (!check_stop(philo->data) && !philo_full(philo->data, philo))
	{
		if (!philo_eating(philo))
			break ;
		if (check_stop(philo->data))
			break ;
		print_info(philo->data, philo->index, "is sleeping");
		smart_sleep(philo->data->tt_sleep, philo);
		if (check_stop(philo->data))
			break ;
		think_smart(philo, philo->data);
	}
	return (NULL);
}

bool	philo_eating(t_philo *philo)
{
	if (check_stop(philo->data))
		return (false);
	if (philo->index % 2 == 1 && !check_stop(philo->data))
	{
		pthread_mutex_lock(&philo->data->forks[philo->l_fork]);
		print_info(philo->data, philo->index, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[philo->r_fork]);
		print_info(philo->data, philo->index, "has taken a fork");
	}
	else if (!check_stop(philo->data))
	{
		pthread_mutex_lock(&philo->data->forks[philo->r_fork]);
		print_info(philo->data, philo->index, "has taken a fork");
		pthread_mutex_lock(&philo->data->forks[philo->l_fork]);
		print_info(philo->data, philo->index, "has taken a fork");
	}
	update_meal_data(philo);
	print_info(philo->data, philo->index, "is eating");
	smart_sleep(philo->data->tt_eat, philo);
	pthread_mutex_unlock(&philo->data->forks[philo->r_fork]);
	pthread_mutex_unlock(&philo->data->forks[philo->l_fork]);
	return (true);
}

void	update_meal_data(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal_time = time_passed(philo->data);
	pthread_mutex_lock(&philo->nb_meal_mutex);
	philo->meal_eaten++;
	pthread_mutex_unlock(&philo->nb_meal_mutex);
	pthread_mutex_unlock(&philo->last_meal_mutex);
}

void	print_info(t_data *data, int index, char *message)
{
	if (check_stop(data))
		return ;
	pthread_mutex_lock(&data->print_mutex);
	if (!check_stop(data))
		printf("%li %d %s\n", time_passed(data), index, message);
	pthread_mutex_unlock(&data->print_mutex);
}

bool	check_stop(t_data *data)
{
	bool	val;

	pthread_mutex_lock(&data->stop_mutex);
	val = data->stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (val);
}
