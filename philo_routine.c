/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcoskune <mcoskune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:30:53 by mcoskune          #+#    #+#             */
/*   Updated: 2024/11/06 15:59:29 by mcoskune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	action_routine(t_philo *philo, t_prof *prof)
{
	int	i;

	if (prof->prof_id % 2 == 0)
	{
		i = prof->fork[0];
		prof->fork[0] = prof->fork[1];
		prof->fork[1] = i;
		i = 0;
	}
	else if (prof->prof_id == philo->num_of_philo)
		i = 1;
	else
		i = 2;
	while (1)
	{
		if (check_death(philo))
			return ;
		if (i % 3 == 0)
			eat_routine(philo, prof);
		else if (i % 3 == 1)
			think_routine(philo, prof);
		else
			sleep_routine(philo, prof);
		i++;
	}
}

static void	lone_philo_routine(t_philo *philo, t_prof *prof)
{
	pthread_mutex_lock(&philo->fork_mutex[prof->fork[0]]);
	print_msg(philo, prof->prof_id, get_cur_time(philo), "has taken a fork");
	pthread_mutex_unlock(&philo->fork_mutex[prof->fork[0]]);
	while (1)
	{
		if (check_death(philo))
			return ;
		mini_waits(philo, philo->dt_sleep);
	}
	return ;
}

static int	start_sync(t_philo *philo, t_prof *prof)
{
	pthread_mutex_lock(&philo->start_sig_mutex);
	if (philo->start_signal == false)
	{
		pthread_mutex_unlock(&prof->philo_main->start_sig_mutex);
		return (1);
	}
	pthread_mutex_unlock(&prof->philo_main->start_sig_mutex);
	pthread_mutex_lock(&prof->meal_mutex);
	prof->last_meal = get_cur_time(philo);
	pthread_mutex_unlock(&prof->meal_mutex);
	return (0);
}

void	*philo_routine(void *argv)
{
	t_prof	*prof;
	t_philo	*philo;

	prof = (t_prof *)argv;
	philo = prof->philo_main;
	if (start_sync(philo, prof) != 0)
		return (NULL);
	if (philo->num_of_philo == 1)
		lone_philo_routine(philo, prof);
	else
		action_routine(philo, prof);
	return (NULL);
}
