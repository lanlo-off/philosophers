/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:31:58 by llechert          #+#    #+#             */
/*   Updated: 2025/11/03 16:28:20 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>

typedef struct s_philo
{
	int		index;//position
	bool	l_fork;//fork de gauche dispo
	bool	r_fork;//fork de droite dispo
	int		l_neighbour;//index du voisin de gauche
	int		r_neighbour;//index du voisin de droite
	long	last_meal_time;
	
}	t_philo;

typedef struct s_args
{
	int	nb_philo;
	int	tt_die;
	int	tt_eat;
	int	tt_sleep;
	int	tt_think;
	int	goal;
}	t_args;

/*main.c*/
int		main(int ac, char **av);

/*parser.c*/
bool	parse_args(t_args *arg, char **av, int ac);
void	fill_arg(t_args *arg, long stock[5]);
long	ft_atol(char *str);


#endif