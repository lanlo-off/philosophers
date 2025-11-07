/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miscellaneous.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 10:09:03 by llechert          #+#    #+#             */
/*   Updated: 2025/11/07 17:23:02 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

//une fonction qui calcule le temps passe depuis le debut avec gettimeofday() ?
//Stocker quelque part l'heure de demarrage car elle va servir de T0

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long	time_passed(t_monitor *monitor)
{
	return (get_time_ms() - monitor->start_time);
}

void	smart_sleep(int time_to_sleep, t_philo *philo)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < time_to_sleep)
	{
		if (check_stop(philo) == true)
			break ;
		usleep(100);
	}
}

/**
 * @brief On definit les fourchettes telles que
 * la fourchette a droite est toujours la fork du meme indice que le philo
 * celle a gauche est l'indice superieur sauf pour le dernier philo = 1
 * 
 * @param philo 
 */
void	def_left_n_right(t_philo *philo)
{
	if (philo->index != philo->arg->nb_philo)
		philo->l_fork = philo->index;
	else
		philo->l_fork = 0;
	philo->r_fork = philo->index - 1;
}

void	join_threads(t_monitor *monitor, t_philo *philo)
{
	int	i;
	
	i = 0;
	while (i < monitor->arg->nb_philo)
	{
		pthread_join(philo[i].p, NULL);
		i++;
	}
	pthread_join(monitor->m, NULL);
}