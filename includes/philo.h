/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:31:58 by llechert          #+#    #+#             */
/*   Updated: 2025/11/05 18:07:39 by llechert         ###   ########.fr       */
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
# include <sys/time.h>
# include <fcntl.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>

typedef struct s_philo
{
	pthread_t		p;
	int				index;
	long			last_meal_time;
	pthread_mutex_t	last_meal_mutex;
	int				meal_eaten;
	pthread_mutex_t	nb_meal_mutex;
	t_args			*arg;
}	t_philo;

typedef struct s_monitor
{
	pthread_t		m;
	pthread_mutex_t	start_mutex;
	bool			abort;
	pthread_mutex_t	abort_mutex;
	bool			stop;
	pthread_mutex_t	stop_mutex;
	long			start_time;
	t_args			*arg;
	pthread_mutex_t	**forks;
}	t_monitor;

typedef struct s_args
{
	int	nb_philo;
	int	tt_die;
	int	tt_eat;
	int	tt_sleep;
	int	tt_think;
	int	goal;
}	t_args;

/*cleaning.c*/
void	clean_mutex_struct(t_philo	*philo, int index, int mode);
void	clean_mutex_tab(pthread_mutex_t *forks, int nb);

/*init.c*/
void	init_all(t_args *arg);
bool	init_philo(t_philo *philo, t_args *arg);
bool	init_forks(pthread_mutex_t *forks, t_args *arg);
void	init_process(t_philo *philo, pthread_mutex_t *forks, t_args *arg);
void	init_threads(t_monitor *monitor, t_philo *philo,
	pthread_mutex_t forks, t_args *args);

/*main.c*/
int		main(int ac, char **av);

/*parser.c*/
bool	parse_args(t_args *arg, char **av, int ac);
void	fill_arg(t_args *arg, long stock[5]);
long	ft_atol(char *str);


#endif