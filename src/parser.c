/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llechert <llechert@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:08:02 by llechert          #+#    #+#             */
/*   Updated: 2025/11/03 16:24:41 by llechert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	parse_args(t_args *arg, char **av, int ac)
{
	int		i;
	long	stock[5];

	i = 0;
	while (i < 4)
	{
		stock[i] = ft_atol(av[i + 1]);
		printf("stock[%d]: %ld\n", i, stock[i]);
		if (stock[i] <= 0 || stock[i] > INT_MAX)
			return(false);
		i++;
	}
	if (ac == 6)
	{
		stock[i] = ft_atol(av[i + 1]);
		printf("stock[%d]: %ld\n", i, stock[i]);
		if (stock[i] <= 0 || stock[i] > INT_MAX)
			return(false);
	}
	else 
		stock[i] = -1;
	fill_arg(arg, stock);
	return (true);
}

void	fill_arg(t_args *arg, long stock[5])
{
	arg->nb_philo = (int)stock[0];
	arg->tt_die = (int)stock[1];
	arg->tt_eat = (int)stock[2];
	arg->tt_sleep = (int)stock[3];
	arg->tt_think = 0;//trouver une formule permettant de donner le temps a ceux qui n'ont pas mange d'avoir la priorite
	arg->goal = (int)stock[4];
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
