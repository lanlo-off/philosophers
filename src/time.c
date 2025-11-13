/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:21:11 by llechert          #+#    #+#             */
/*   Updated: 2025/11/13 18:05:34 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long	time_passed(t_data *data)
{
	long	time;

	time = get_time_ms() - data->start_time;
	return (time);
}

void	smart_sleep(int time_to_sleep, t_philo *philo)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < time_to_sleep)
	{
		if (check_stop(philo->data) == true)
			break ;
		usleep(100);
	}
}
