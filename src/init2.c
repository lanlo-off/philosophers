/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:27:31 by llechert          #+#    #+#             */
/*   Updated: 2025/11/07 10:27:32 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief On definit les fourchettes telles que
 * la fourchette a droite est toujours la fork du meme indice que le philo
 * celle a gauche est l'indice superieur sauf pour le dernier philo = 1
 * 
 * @param philo 
 */
void	def_left_n_right(t_philo philo)
{
	if (philo.index != philo.arg->nb_philo)
		philo.l_fork = philo.index + 1;
	else
		philo.l_fork = 1;
	philo.r_fork = philo.index;
}