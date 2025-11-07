/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:18:22 by llechert          #+#    #+#             */
/*   Updated: 2025/11/07 17:45:57 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief commence par verifier si le philo a deja mange assez
 * et s'arrete si on est au n-ieme philo plein
 * Puis si verifie s'il est mort et s'arrete si oui
 * 
 * @param arg
 */
void	*m_routine(void *arg)
{
	t_philo	*philo;
	int		i;
	int 	goal_count;

	philo = (t_philo *)arg;
	goal_count = 0;
	while (!check_stop(philo))
	{
		i = 0;
		goal_count = 0;
		usleep(50);//temps defini arbitrairement
		while (i < philo->arg->nb_philo)
		{
			pthread_mutex_lock(&philo[i].nb_meal_mutex);
			if (philo[i].meal_eaten == philo[i].arg->goal)
				goal_count++;
			pthread_mutex_unlock(&philo[i].nb_meal_mutex);
			if (goal_count == philo[i].arg->nb_philo)
			{
				set_stop(&philo[i], true);
				break ;
			}
			pthread_mutex_lock(&philo[i].last_meal_mutex);
			if (time_passed(philo[i].monitor) - philo[i].last_meal_time >= philo->arg->tt_die)//creer une fonction qui recupere le current time depuis le debut des threads (suppose d'avoir le starting time stocke qqpart)
			{
				pthread_mutex_unlock(&philo[i].last_meal_mutex);//ou est-ce que je le mets au tout debut du if juste apres le calcul ?
				print_info(&philo[i], "died");
				set_stop(&philo[i], true);
				break ;
			}
			pthread_mutex_unlock(&philo[i].last_meal_mutex);
			i++;
		}
	}
	return (NULL);
}

void	set_stop(t_philo *philo, bool val)
{
	pthread_mutex_lock(&philo->monitor->stop_mutex);
	philo->monitor->stop = val;
	pthread_mutex_unlock(&philo->monitor->stop_mutex);
}
