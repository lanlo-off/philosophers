/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:24:14 by llechert          #+#    #+#             */
/*   Updated: 2025/11/07 14:20:31 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_args(t_args *arg)
{
	printf("nb_philo: %d\n", arg->nb_philo);
	printf("tt_die: %d\n", arg->tt_die);
	printf("tt_eat: %d\n", arg->tt_eat);
	printf("tt_sleep: %d\n", arg->tt_sleep);
	printf("goal: %d\n", arg->goal);
}

int	main(int ac, char **av)
{
	t_args	*arg;

	if (ac < 5 || ac > 6)
	{
		printf("Wrong number of arguments!");
		return (0);
	}
	arg = malloc(sizeof(t_args));
	if (!arg)
		return (1);
	if (parse_args(arg, av, ac) == false)
	{
		printf("Wrong arguments!");
		free(arg);
		return (0);
	}
	print_args(arg);
	init_all(arg);
	free(arg);
	return (0);
}
