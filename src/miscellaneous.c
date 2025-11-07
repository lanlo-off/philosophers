/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miscellaneous.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 10:09:03 by llechert          #+#    #+#             */
/*   Updated: 2025/11/07 10:34:15 by llechert         ###   ########.fr       */
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

void	smart_sleep(int time_to_sleep, t_monitor *monitor)
{
	long	start;

	start = get_time_ms();
	while (!monitor->stop && (get_time_ms() - start < time_to_sleep))
		usleep(100);
}
