/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:31:58 by llechert          #+#    #+#             */
/*   Updated: 2025/11/07 10:26:48 by llechert         ###   ########.fr       */
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
	int				l_fork;
	int				r_fork;
	long			last_meal_time;
	pthread_mutex_t	last_meal_mutex;
	int				meal_eaten;
	pthread_mutex_t	nb_meal_mutex;
	t_args			*arg;
	t_monitor		*monitor;
}	t_philo;

typedef struct s_monitor
{
	pthread_t		m;
	pthread_mutex_t	start_mutex;
	bool			abort;//est-ce utile d'avoir un abort en plus du stop ?
	pthread_mutex_t	abort_mutex;
	bool			stop;
	pthread_mutex_t	stop_mutex;
	long			start_time;
	t_args			*arg;
	pthread_mutex_t	**forks;
	pthread_mutex_t	print_mutex;
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
void	init_monitor(t_philo *philo, pthread_mutex_t *forks, t_args *arg);
void	init_threads(t_monitor *monitor, t_philo *philo);

/*init2.c*/
void	def_left_n_right(t_philo philo);

/*main.c*/
int		main(int ac, char **av);

/*monitor_routine.c*/
void	m_routine(t_philo *philo);
void	set_stop(t_philo philo, bool val);

/*parser.c*/
bool	parse_args(t_args *arg, char **av, int ac);
void	fill_arg(t_args *arg, long stock[5]);
long	ft_atol(char *str);

/*philo_routine.c*/
void	p_routine(t_philo philo);

#endif