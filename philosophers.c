/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcoskune <mcoskune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 11:47:57 by mcoskune          #+#    #+#             */
/*   Updated: 2024/11/06 16:08:13 by mcoskune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	mini_waits(t_philo *philo, int delta_t)
{
	long long	t_end_wait;

	t_end_wait = get_cur_time(philo) + delta_t;
	while (get_cur_time(philo) < t_end_wait)
	{
		usleep(10);
	}
}

static void	join_philosophers(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->num_of_philo)
	{
		pthread_join(philo->profs[i]->prof, NULL);
		i++;
	}
	printf("All philosophers joined to `main` thread!\n");
}

static int	create_judge(t_philo *philo)
{
	if (pthread_create(&philo->judge_dredd, NULL, judge_routine, philo) != 0)
	{
		philo->kill_signal = true;
		pthread_mutex_unlock(&philo->start_sig_mutex);
		pthread_mutex_unlock(&philo->kill_sig_mutex);
		join_philosophers(philo);
		return (1);
	}
	return (0);
}

static int	create_profs(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->num_of_philo)
	{
		if (pthread_create(&philo->profs[i]->prof, NULL, \
				philo_routine, philo->profs[i]) != 0)
		{
			philo->kill_signal = true;
			philo->num_of_philo = i;
			pthread_mutex_unlock(&philo->start_sig_mutex);
			pthread_mutex_unlock(&philo->kill_sig_mutex);
			join_philosophers(philo);
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_philo	philo;

	if (input_validate(ac, av))
		return (EXIT_FAILURE);
	if (clean_initialize(&philo, ac, av))
		return (EXIT_FAILURE);
	pthread_mutex_lock(&philo.start_sig_mutex);
	pthread_mutex_lock(&philo.kill_sig_mutex);
	if (create_profs(&philo) != 0)
		return (exit_cleanup(&philo, 0, 0), EXIT_FAILURE);
	if (create_judge(&philo) != 0)
		return (exit_cleanup(&philo, 0, 0), EXIT_FAILURE);
	pthread_mutex_unlock(&philo.kill_sig_mutex);
	philo.start_signal = true;
	philo.t_start = get_cur_time(&philo);
	pthread_mutex_unlock(&philo.start_sig_mutex);
	join_philosophers(&philo);
	pthread_join(philo.judge_dredd, NULL);
	printf("DREDD joined to `main` thread!\n");
	exit_cleanup(&philo, 0, 0);
	return (EXIT_SUCCESS);
}
