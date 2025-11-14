/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 14:41:39 by llechert          #+#    #+#             */
/*   Updated: 2025/11/14 16:07:26 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	think_smart(t_philo *philo, t_data *data)
{
	long	tt_think;

	print_info(philo->data, philo->index, "is thinking");
	tt_think = 0;
	if (data->nb_philo % 2 == 1 && \
		data->tt_die > (2 * data->tt_eat + data->tt_sleep))
	{
		tt_think = 2 * data->tt_eat - data->tt_sleep;
		if (tt_think < 0)
			tt_think = 0;
		smart_sleep(tt_think, philo);
	}
}
