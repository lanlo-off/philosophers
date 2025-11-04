/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:35:06 by llechert          #+#    #+#             */
/*   Updated: 2025/11/03 16:29:21 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	solver(t_args *arg)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo) * arg->nb_philo);
	if (!philo)
		return (NULL);
	init_philo(philo, arg);
	//start_eating
}

void	init_philo(t_philo *philo, t_args *arg)
{
	int	i;
	
	i = 0;
	while (i < arg->nb_philo)//cas general
	{
		philo[i].index = i;
		philo[i].l_fork = true;
		philo[i].r_fork = true;
		philo[i].l_neighbour = i - 1;
		philo[i].r_neighbour = i + 1;
		philo[i].last_meal_time = 0;
		i++;
	}
	philo[0].l_neighbour = arg->nb_philo;
	philo[arg->nb_philo].r_neighbour = 1;
}