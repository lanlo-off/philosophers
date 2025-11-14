/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 11:06:27 by llechert          #+#    #+#             */
/*   Updated: 2025/11/14 16:05:37 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac < 5 || ac > 6)
	{
		printf("Wrong number of arguments!\n");
		return (0);
	}
	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	if (parse_args(data, av, ac) == false)
	{
		printf("Wrong arguments!\n");
		free(data);
		return (0);
	}
	start_process(data);
	return (0);
}

void	start_process(t_data *data)
{
	if (!init_data(data))
		clean_and_exit(data, 0);
	if (!init_forks(data))
		clean_and_exit(data, 1);
	if (!init_philo(data))
		clean_and_exit(data, 2);
	init_threads(data);
}
