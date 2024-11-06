/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcoskune <mcoskune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:02:26 by mcoskune          #+#    #+#             */
/*   Updated: 2024/11/06 16:00:28 by mcoskune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->kill_sig_mutex);
	if (philo->kill_signal == true)
	{
		pthread_mutex_unlock(&philo->kill_sig_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->kill_sig_mutex);
	return (false);
}

void	eat_routine(t_philo *philo, t_prof *prof)
{
	if (check_death(philo))
		return ;
	pthread_mutex_lock(&philo->fork_mutex[prof->fork[0]]);
	print_msg(philo, prof->prof_id, get_cur_time(philo), "has taken a fork");
	pthread_mutex_lock(&philo->fork_mutex[prof->fork[1]]);
	print_msg(philo, prof->prof_id, get_cur_time(philo), "has taken a fork");
	print_msg(philo, prof->prof_id, get_cur_time(philo), " is eating");
	pthread_mutex_lock(&prof->meal_mutex);
	prof->last_meal = get_cur_time(philo);
	prof->num_eaten++;
	pthread_mutex_unlock(&prof->meal_mutex);
	mini_waits(philo, philo->dt_eat);
	pthread_mutex_unlock(&philo->fork_mutex[prof->fork[1]]);
	pthread_mutex_unlock(&philo->fork_mutex[prof->fork[0]]);
}

void	think_routine(t_philo *philo, t_prof *prof)
{
	int	t_think;

	if (check_death(philo))
		return ;
	t_think = (philo->dt_die - (get_cur_time(philo) - prof->last_meal) \
														- philo->dt_eat) / 2;
	if (t_think <= 0)
		t_think = 10;
	else if (t_think >= philo->dt_eat)
		t_think = philo->dt_eat / 2;
	print_msg(philo, prof->prof_id, get_cur_time(philo), "is thinking");
	mini_waits(philo, t_think);
}

void	sleep_routine(t_philo *philo, t_prof *prof)
{
	if (check_death(philo))
		return ;
	print_msg(philo, prof->prof_id, get_cur_time(philo), "is sleeping");
	mini_waits(philo, philo->dt_sleep);
}