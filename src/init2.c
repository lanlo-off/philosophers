/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:27:31 by llechert          #+#    #+#             */
/*   Updated: 2025/11/07 14:45:55 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	assign_monitor(t_monitor *monitor, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < monitor->arg->nb_philo)
	{
		philo[i].monitor = monitor;
		i++;
	}
}
