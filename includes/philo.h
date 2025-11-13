/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 14:31:58 by llechert          #+#    #+#             */
/*   Updated: 2025/11/13 18:05:55 by llechert         ###   ########.fr       */
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

typedef struct s_philo t_philo;
typedef struct s_data
{
	int	nb_philo;
	int	tt_die;
	int	tt_eat;
	int	tt_sleep;
	int	tt_think;
	int	goal;
	pthread_t		m;
	pthread_mutex_t	start_mutex;
	bool			stop;
	pthread_mutex_t	stop_mutex;
	long			start_time;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*forks;
	t_philo			*philo;
}	t_data;

struct s_philo
{
	pthread_t		p;
	int				index;
	int				l_fork;
	int				r_fork;
	long			last_meal_time;
	pthread_mutex_t	last_meal_mutex;
	int				meal_eaten;
	pthread_mutex_t	nb_meal_mutex;
	t_data			*data;
};

/*cleaning.c*/
void	clean_and_exit(t_data *data, int mode);
void	clean_mutex_philo(t_philo	*philo, int index, int mode);
void	clean_mutex_tab(pthread_mutex_t *forks, int nb);
void	join_threads(t_data *data);

/*init.c*/
bool	init_data(t_data *data);
bool	init_forks(t_data *data);
bool	init_philo(t_data *data);
void	fill_philo(t_data *data, int i);
void	init_threads(t_data *data);

/*main.c*/
int		main(int ac, char **av);
void	start_process(t_data *data);

/*time.c*/
long	get_time_ms(void);
long	time_passed(t_data *data);
void	smart_sleep(int time_to_sleep, t_philo *philo);

/*monitor.c ==> a modifier */
void	*m_rout(void *arg);
bool	philo_full(t_data *data, t_philo *philo);
bool	all_philo_full(t_data *data, int goal_count);
void	set_stop(t_data *data, bool val);
bool	check_death(t_data *data, t_philo *philo);

/*parser.c*/
bool	parse_args(t_data *data, char **av, int ac);
void	fill_data(t_data *data, long *stock);
long	ft_atol(char *str);

/*philo_routine.c*/
void	*p_rout(void *arg);
bool	philo_eating(t_philo *philo);
void	update_meal_data(t_philo *philo);
void	print_info(t_data *data, int index, char *message);
bool	check_stop(t_data *data);
#endif