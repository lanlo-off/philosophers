/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:12:11 by llechert          #+#    #+#             */
/*   Updated: 2025/11/05 15:26:45 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief sert a detruire les mutex de mon tableau de structure philo
 * 
 * @param philo tableau de structure dont il faut vider les mutex
 * @param index Jusqu'a quel indice les mutex ont ete crees dans le tableau
 * @param mode 0 = pas d'erreur dans ces mutex ; 
 * 1 si c'est un mutex de last_meal qui a merde ; 
 * 2 si c'est un mutex de meal_eaten
 */
void	clean_mutex_struct(t_philo	*philo, int index, int mode)
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
	clean_mutex_struct(philo, arg->nb_philo, 0);
	clean_mutex_tab(forks, arg->nb_philo);
	free(philo);
	free(forks);
}