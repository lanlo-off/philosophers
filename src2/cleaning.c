/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 11:53:01 by llechert          #+#    #+#             */
/*   Updated: 2025/11/13 12:55:43 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo2.h"

/**
 * @brief fonction de cleaning en cascade
 * en cas d'erreur lors de l'initialisation d'un element
 * on cleane cet element dans la fonction d'init avant d'arriver ici pour
 * clean ce qui avait ete correctement initialise
 * 
 * @param data 
 * @param mode correspond au moment de l'erreur rencontree
 * 0 = init data
 * 1 = forks
 * 2 = philos
 * 3 = threads
 * 4 = pas d'erreur 
 */
void	clean_and_exit(t_data *data, int mode)
{
	if (mode == 4)
	{
		join_threads(t_data *data);
	}
	if (mode >= 3)//on cleane les threads avant ici
	{
		clean_mutex_philo(data->philo, data->nb_philo, 1);//clean tous les philos
		free(data->philo);
	}
	if (mode >= 2)//on cleane tous les philos avant d'arriver ici
	{
		clean_mutex_tab(data->forks, data->nb_philo);//destroy toutes les forks
		free(data->forks);
	}
	if (mode >= 1)//on cleane les forks avant ici
	{
		pthread_mutex_destroy(&data->start_mutex);
		pthread_mutex_destroy(&data->stop_mutex);
		pthread_mutex_destroy(&data->print_mutex);
	}
	free(data);
	exit(mode != 3);
}

void	clean_mutex_philo(t_philo *philo, int index, int mode)
{
	int	i;

	i = 0;
	while (i < index)
	{
		pthread_mutex_destroy(&philo[i].last_meal_mutex);
		pthread_mutex_destroy(&philo[i].nb_meal_mutex);
		i++;
	}
	if (mode == 2)
		pthread_mutex_destroy(&philo[i].last_meal_mutex);
}

/**
 * @brief Sert a destroy tous les mutex d'un tableau de mutex
 * 
 * @param forks tableau impacte
 * @param nb nb d'elements dans le tableau
 */
void	clean_mutex_tab(pthread_mutex_t *forks, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		pthread_mutex_destroy(&forks[i]);
		i++;
	}
}
/**
 * @brief Pour clean et free le tab de struct philo et le tableau de mutex
 * a la fin du programme
 * 
 * @param philo tab de philo
 * @param forks tab de mutex
 * @param arg pour avoir le nb_philo
 */
void	clean_all(t_philo *philo, pthread_mutex_t *forks, t_args *arg)
{
	clean_mutex_philo(philo, arg->nb_philo, 0);
	clean_mutex_tab(forks, arg->nb_philo);
	free(philo);
	free(forks);
}