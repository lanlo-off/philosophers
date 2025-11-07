/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 11:51:48 by llechert          #+#    #+#             */
/*   Updated: 2025/11/07 17:45:47 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*p_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->index % 2 == 1)
		usleep(50);
	while (!check_stop(philo))
	{
		if (philo_eating(philo) == false && !check_stop(philo))//eat
			break ;
		if (check_stop(philo))
			break ;
		print_info(philo, "is sleeping");
		smart_sleep(philo->arg->tt_sleep, philo);//sleep
		if (check_stop(philo))
			break ;
		print_info(philo, "is thinking");//thinks : pas de duree definie car il think jusqu'a reussir a prendre la premiere fourchette
	}
	return (NULL);
}

bool	check_stop(t_philo *philo)
{
	bool	val;
	
	pthread_mutex_lock(&philo->monitor->stop_mutex);
	val = philo->monitor->stop;
	pthread_mutex_unlock(&philo->monitor->stop_mutex);
	return (val);
}

bool	philo_eating(t_philo *philo)
{
	if (check_stop(philo))
		return (false);
	if (philo->index % 2 == 0 && !check_stop(philo))
	{
		pthread_mutex_lock(&philo->monitor->forks[philo->l_fork]);
		print_info(philo, "has taken a fork");
		pthread_mutex_lock(&philo->monitor->forks[philo->r_fork]);
		print_info(philo, "has taken a fork");
	}
	else if (!check_stop(philo))
	{
		pthread_mutex_lock(&philo->monitor->forks[philo->r_fork]);
		print_info(philo, "has taken a fork");
		pthread_mutex_lock(&philo->monitor->forks[philo->l_fork]);
		print_info(philo, "has taken a fork");
	}
	update_meal_data(philo);
	print_info(philo, "is eating");
	smart_sleep(philo->arg->tt_eat, philo);//sleep en faisant des pause pour check le stop
	pthread_mutex_unlock(&philo->monitor->forks[philo->r_fork]);//si stop dans la pause du smart sleep, on n'unlock pas les forks
	pthread_mutex_unlock(&philo->monitor->forks[philo->l_fork]);
	if (check_stop(philo))
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

/**
 * @brief afficher les messages take a fork et eat
 * 
 * @param mode 0 pour fork 1 pour eat 2 pour sleep 3 pour think 4 pour die
 * @param philo envoyer le t_philo entier
 */
void	print_info(t_philo *philo, char *message)
{
	pthread_mutex_lock(&philo->monitor->print_mutex);
	if (!check_stop(philo))
		printf("%li %d %s\n", time_passed(philo->monitor), philo->index, message);
	pthread_mutex_unlock(&philo->monitor->print_mutex);
}
