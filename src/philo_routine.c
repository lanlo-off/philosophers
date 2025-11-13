/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:12:56 by llechert          #+#    #+#             */
/*   Updated: 2025/11/13 17:37:07 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*p_rout(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->index % 2 == 1)
		usleep(50);
	while (!check_stop(philo->data))
	{
		if (philo_eating(philo) == false && !check_stop(philo->data))//eat
			break ;
		if (check_stop(philo->data))
			break ;
		print_info(philo->data, philo->index, "is sleeping");
		smart_sleep(philo->data->tt_sleep, philo);//sleep
		if (check_stop(philo->data))
			break ;
		print_info(philo->data, philo->index, "is thinking");//thinks : pas de duree definie car il think jusqu'a reussir a prendre la premiere fourchette
	}
	return (NULL);
}

bool	philo_eating(t_philo *philo)
{
	if (check_stop(philo->data))
		return (false);
	if (philo->index % 2 == 0 && !check_stop(philo->data))
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
	smart_sleep(philo->data->tt_eat, philo);//sleep en faisant des pause pour check le stop
	pthread_mutex_unlock(&philo->data->forks[philo->r_fork]);//si stop dans la pause du smart sleep, on n'unlock pas les forks
	pthread_mutex_unlock(&philo->data->forks[philo->l_fork]);
	if (check_stop(philo->data))//pas certain de ce truc
		return (false);
	return (true);
}

void	update_meal_data(t_philo *philo)
{
	pthread_mutex_lock(&philo->last_meal_mutex);//update du last meal time
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&philo->last_meal_mutex);
	pthread_mutex_lock(&philo->nb_meal_mutex);//update du nb de meal
	philo->meal_eaten++;
	pthread_mutex_unlock(&philo->nb_meal_mutex);
}

void	print_info(t_data *data, int index, char *message)
{
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
