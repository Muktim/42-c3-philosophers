/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   judge_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcoskune <mcoskune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 19:58:51 by mcoskune          #+#    #+#             */
/*   Updated: 2024/10/29 00:29:29 by mcoskune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	check_all_eaten(t_philo *philo, t_prof *prof, int *all_eaten)
{
	if (prof->num_eaten >= philo->min_eat_req)
	{
		(*all_eaten)++;
		if (*all_eaten == philo->num_of_philo)
		{
			pthread_mutex_lock(&philo->print_mutex);
			pthread_mutex_lock(&philo->kill_sig_mutex);
			philo->kill_signal = true;
			printf("Dinner is served\n");
			pthread_mutex_unlock(&philo->kill_sig_mutex);
			pthread_mutex_unlock(&philo->print_mutex);
			pthread_mutex_unlock(&prof->meal_mutex);
			return (0);
		}
	}
	return (1);
}

static int	check_death(t_philo *philo, t_prof *prof, int i)
{
	if (get_current_time(philo) - prof->last_meal >= philo->dt_die)
	{
		pthread_mutex_lock(&philo->print_mutex);
		pthread_mutex_lock(&philo->kill_sig_mutex);
		philo->kill_signal = true;
		printf("%lld %d died\n", get_current_time(philo), i + 1);
		pthread_mutex_unlock(&philo->kill_sig_mutex);
		pthread_mutex_unlock(&philo->print_mutex);
		pthread_mutex_unlock(&philo->profs[i]->meal_mutex);
		return (1);
	}
	return (0);
}

static void	dredd_cycle(t_philo *philo)
{
	int	i;
	int	all_eaten;

	while (1)
	{
		i = 0;
		all_eaten = 0;
		while (i < philo->num_of_philo)
		{
			pthread_mutex_lock(&philo->profs[i]->meal_mutex);
			if (check_death(philo, philo->profs[i], i) != 0)
				return ;
			if (philo->min_eat_req != -1)
			{
				if (check_all_eaten(philo, philo->profs[i], &all_eaten) == 0)
					return ;
			}
			pthread_mutex_unlock(&philo->profs[i++]->meal_mutex);
		}
	}
}

void	*judge_routine(void *argv)
{
	t_philo	*philo;

	philo = (t_philo *)argv;
	pthread_mutex_lock(&philo->start_sig_mutex);
	if (philo->start_signal == false)
	{
		pthread_mutex_unlock(&philo->start_sig_mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&philo->start_sig_mutex);
	dredd_cycle(philo);
	return (NULL);
}
