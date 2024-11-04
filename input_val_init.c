/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_val_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcoskune <mcoskune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 00:31:35 by mcoskune          #+#    #+#             */
/*   Updated: 2024/11/04 17:06:16 by mcoskune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	init_mutex(t_philo *philo)
{
	int	i;

	if (pthread_mutex_init(&philo->print_mutex, NULL) != 0)
		return (exit_cleanup(philo, 1, 0), 1);
	if (pthread_mutex_init(&philo->kill_sig_mutex, NULL) != 0)
		return (exit_cleanup(philo, 2, 0), 2);
	if (pthread_mutex_init(&philo->start_sig_mutex, NULL) != 0)
		return (exit_cleanup(philo, 3, 0), 2);
	i = 0;
	while (i < philo->num_of_philo)
	{
		if (pthread_mutex_init(&philo->fork_mutex[i], NULL) != 0)
			return (exit_cleanup(philo, 5, i), 3);
		if (pthread_mutex_init(&philo->profs[i]->meal_mutex, NULL) != 0)
			return (exit_cleanup(philo, 6, i), 4);
		i++;
	}
	return (0);
}

static int	init_profs(t_philo *philo)
{
	int	i;

	philo->profs = malloc(sizeof(t_prof *) * philo->num_of_philo);
	if (philo->profs == NULL)
		return (1);
	i = 0;
	while (i < philo->num_of_philo)
	{
		philo->profs[i] = malloc (sizeof(t_prof));
		if (philo->profs[i] == NULL)
			return (2);
		philo->profs[i]->prof_id = i + 1;
		philo->profs[i]->num_eaten = 0;
		philo->profs[i]->last_meal = 0;
		philo->profs[i]->fork[0] = i;
		philo->profs[i]->fork[1] = i + 1;
		if (philo->profs[i]->prof_id %2 == 0) //&& philo->num_of_philo != 2
		{
			philo->profs[i]->fork[0] = i + 1;
			philo->profs[i]->fork[1] = i;
		}
		if (i == philo->num_of_philo - 1)
			philo->profs[i]->fork[1] = 0;
		philo->profs[i]->philo_main = philo;
		i++;
	}
	return (0);
}

int	clean_initialize(t_philo *philo, int ac, char **av)
{
	if (philo == NULL || av == NULL || *av == NULL)
		return (1);
	philo->num_of_philo = ft_atoi(av[1]);
	philo->t_start = 0;
	philo->dt_die = ft_atoi(av[2]);
	philo->dt_eat = ft_atoi(av[3]);
	philo->dt_sleep = ft_atoi(av[4]);
	philo->dt_think = (philo->dt_die - philo->dt_eat - philo->dt_sleep);
	philo->min_eat_req = -1;
	if (ac == 6)
		philo->min_eat_req = ft_atoi(av[5]);
	philo->start_signal = false;
	philo->kill_signal = false;
	philo->profs = NULL;
	philo->fork_mutex = malloc(sizeof(pthread_mutex_t) * philo->num_of_philo);
	if (philo->fork_mutex == NULL)
		return (1);
	if (init_profs(philo))
	{
		exit_cleanup(philo, 1, 0);
		return (2);
	}
	if (init_mutex(philo))
		return (3);
	return (0);
}

int	input_validate(int ac, char **av)
{
	int	i;
	int	j;

	if (ac != 5 && ac != 6)
		return (msg("Error - AC - Why is this AC so spicy?\n"), 1);
	i = 1;
	while (i < ac)
	{
		j = 0;
		if (av[i][0] == '\0')
			return (msg("Error - AV - Empty hearted evaluators...\n"), 2);
		while (av[i][j] != '\0')
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (msg("Error - AV - Dwarf in Elven army spotted!\n"), 3);
			j++;
		}
		if (ft_atol(av[i]) >= INT_MAX || ft_atol(av[i]) < 0)
			return (msg("Error - AV - These INT are mutated!!\n"), 4);
		i++;
	}
	return (0);
}
