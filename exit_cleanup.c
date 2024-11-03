/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcoskune <mcoskune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 14:51:04 by mcoskune          #+#    #+#             */
/*   Updated: 2024/10/28 17:36:40 by mcoskune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	clear_mutex(t_philo *philo, int stage, int iteration)
{
	int	i;

	if (stage > 1 || stage == 0)
		pthread_mutex_destroy(&philo->print_mutex);
	if (stage > 2 || stage == 0)
		pthread_mutex_destroy(&philo->kill_sig_mutex);
	if (stage > 3 || stage == 0)
		pthread_mutex_destroy(&philo->start_sig_mutex);
	if (stage > 4 || stage == 0)
	{
		// pthread_mutex_destroy(&philo->judge_mutex);
		i = 0;
		while (i < iteration)
		{
			if (stage == 5 && i + 1 == iteration)
				break ;
			pthread_mutex_destroy(&philo->fork_mutex[i]);
			if (stage == 6 && i + 1 == iteration)
				break ;
			pthread_mutex_destroy(&philo->profs[i]->meal_mutex);
			i++;
		}
	}
}

void	exit_cleanup(t_philo *philo, int stage, int iteration)
{
	int	i;

	if (philo == NULL)
		return ;
	if (stage != 1)
	{
		if (stage == 0)
			clear_mutex(philo, stage, philo->num_of_philo);
		else
			clear_mutex(philo, stage, iteration);
	}
	if (philo->profs != NULL)
	{
		i = 0;
		while (i < philo->num_of_philo && philo->profs[i] != NULL)
		{
			free(philo->profs[i]);
			philo->profs[i] = NULL;
			i++;
		}
		free(philo->profs);
	}
	if (philo->fork_mutex != NULL)
		free(philo->fork_mutex);
}
