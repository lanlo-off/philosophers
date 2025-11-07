/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 11:51:48 by llechert          #+#    #+#             */
/*   Updated: 2025/11/07 10:52:53 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	p_routine(t_philo philo)
{
	
	if (philo.index % 2 == 1)
		usleep(500);
	while (!philo.monitor->stop)
	{
		if (check_stop(philo) == true)
			break ;
		philo_eating(philo);//eat
		print_info(philo, "is sleeping");
		smart_sleep(philo.arg->tt_sleep, philo.monitor);//sleep
		print_info(philo, "is thinking");//thinks : pas de duree definie car il think jusqu'a reussir a prendre la premiere fourchette
	}
}

bool	check_stop(t_philo philo)
{
	bool	val;
	
	pthread_mutex_lock(&philo.monitor->stop_mutex);
	val = philo.monitor->stop;
	pthread_mutex_unlock(&philo.monitor->stop_mutex);
	return (val);
}

void	philo_eating(t_philo philo)
{
	if (philo.index % 2 == 1)//try eating depends on %2
	{
		pthread_mutex_lock(&philo.monitor->forks[philo.l_fork]);
		print_info(philo, "has taken a fork");
		pthread_mutex_lock(&philo.monitor->forks[philo.r_fork]);
		print_info(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&philo.monitor->forks[philo.r_fork]);
		print_info(philo, "has taken a fork");
		pthread_mutex_lock(&philo.monitor->forks[philo.l_fork]);
		print_info(philo, "has taken a fork");
	}
	print_info(philo, "is eating");
	smart_sleep(philo.arg->tt_eat, philo.monitor);//sleep en faisant des pause pour check le stop
	pthread_mutex_unlock(&philo.monitor->forks[philo.r_fork]);
	pthread_mutex_unlock(&philo.monitor->forks[philo.l_fork]);
	pthread_mutex_lock(&philo.last_meal_mutex);//update du last meal
	philo.last_meal_time = time_passed(philo.monitor);
	pthread_mutex_unlock(&philo.last_meal_mutex);
}

/**
 * @brief afficher les messages take a fork et eat
 * 
 * @param mode 0 pour fork 1 pour eat 2 pour sleep 3 pour think 4 pour die
 * @param philo envoyer le t_philo entier
 */
void	print_info(t_philo philo, char *message)
{
	pthread_mutex_lock(&philo.monitor->print_mutex);
	printf("%li %d %s", time_passed(philo.monitor), philo.index, message);
	pthread_mutex_unlock(&philo.monitor->print_mutex);
}
