/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 11:14:31 by llechert          #+#    #+#             */
/*   Updated: 2025/11/13 17:30:19 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	parse_args(t_data *data, char **av, int ac)
{
	int		i;
	long	*tmp;

	i = 0;
	tmp = malloc(sizeof(long) * 5);
	if (!tmp)
		return (false);
	while (i < 4)
	{
		tmp[i] = ft_atol(av[i + 1]);
		if (tmp[i] <= 0 || tmp[i] > INT_MAX)
			return (false);
		i++;
	}
	if (ac == 6)
	{
		tmp[i] = ft_atol(av[i + 1]);
		if (tmp[i] <= 0 || tmp[i] > INT_MAX)
			return (false);
	}
	else
		tmp[i] = -1;
	fill_data(data, tmp);
	free(tmp);
	return (true);
}

void	fill_data(t_data *data, long *tmp)
{
	data->nb_philo = (int)tmp[0];
	data->tt_die = (int)tmp[1];
	data->tt_eat = (int)tmp[2];
	data->tt_sleep = (int)tmp[3];
	data->tt_think = 0;//trouver une formule permettant de donner le temps a ceux qui n'ont pas mange d'avoir la priorite
	data->goal = (int)tmp[4];
}

long	ft_atol(char *str)
{
	long	nb;
	int		i;
	int		sign;

	i = 0;
	nb = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i] || !(str[i] >= '0' && str[i] <= '9'))
		return ((long)INT_MAX + 1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + str[i] - 48;
		i++;
	}
	while (str[i] == ' ')
		i++;
	if (str[i])
		return ((long)INT_MAX + 1);
	return (nb * sign);
}
