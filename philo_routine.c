/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcoskune <mcoskune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 12:30:53 by mcoskune          #+#    #+#             */
/*   Updated: 2024/10/29 00:40:00 by mcoskune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
static void	sleep_routine(t_philo *philo, t_prof *prof);

static void	eat_routine(t_philo *philo, t_prof *prof)
{
	pthread_mutex_lock(&philo->kill_sig_mutex);
	if (philo->kill_signal == true)
	{
		pthread_mutex_unlock(&philo->kill_sig_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->kill_sig_mutex);
	pthread_mutex_lock(&philo->fork_mutex[prof->fork[0]]);
	print_msg(philo, prof->prof_id, get_current_time(philo), "got fork - left");
	pthread_mutex_lock(&philo->fork_mutex[prof->fork[1]]);
	print_msg(philo, prof->prof_id, get_current_time(philo), "got fork - right");
	pthread_mutex_lock(&prof->meal_mutex);
	prof->last_meal = get_current_time(philo);
	prof->num_eaten++;
	pthread_mutex_unlock(&prof->meal_mutex);
	print_msg(philo, prof->prof_id, get_current_time(philo), "eating");
	mini_waits(philo, philo->dt_eat);
	pthread_mutex_unlock(&philo->fork_mutex[prof->fork[1]]);
	pthread_mutex_unlock(&philo->fork_mutex[prof->fork[0]]);
	sleep_routine(philo, prof);
}

static void	think_routine(t_philo *philo, t_prof *prof)
{
	int	t_think;
	
	t_think = (philo->dt_die - (get_current_time(philo) - prof->last_meal) - philo->dt_eat) / 2;
	pthread_mutex_lock(&philo->kill_sig_mutex);
	if (philo->kill_signal == true)
	{
		pthread_mutex_unlock(&philo->kill_sig_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->kill_sig_mutex);
	print_msg(philo, prof->prof_id, get_current_time(philo), "thinking");
	mini_waits(philo, t_think);
	eat_routine(philo, prof);
}

static void	sleep_routine(t_philo *philo, t_prof *prof)
{
	pthread_mutex_lock(&philo->kill_sig_mutex);
	if (philo->kill_signal == true)
	{
		pthread_mutex_unlock(&philo->kill_sig_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->kill_sig_mutex);
	print_msg(philo, prof->prof_id, get_current_time(philo), "sleeping");
	mini_waits(philo, philo->dt_eat);
	think_routine(philo, prof);
}

static void	lone_philo_routine(t_philo *philo, t_prof *prof)
{
	pthread_mutex_lock(&philo->kill_sig_mutex);
	if (philo->kill_signal == true)
	{
		pthread_mutex_unlock(&philo->kill_sig_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->kill_sig_mutex);
	pthread_mutex_lock(&philo->fork_mutex[prof->fork[0]]);
	print_msg(philo, prof->prof_id, get_current_time(philo), "got fork - left");
	pthread_mutex_unlock(&philo->fork_mutex[prof->fork[0]]);
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
	prof->last_meal = get_current_time(philo);
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
	else if (philo->num_of_philo % 2 == 0)
	{
		if (prof->prof_id % 2 == 1)
			eat_routine(philo, prof);
		else
			think_routine(philo, prof);
	}
	else
	{
		if (prof->prof_id % 2 == 1 && prof->prof_id != philo->num_of_philo + 1)
			eat_routine(philo, prof);
		else if (prof->prof_id % 2 == 0)
			think_routine(philo, prof);
		else
			sleep_routine(philo, prof);
	}
	return (NULL);
}
