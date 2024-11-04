/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcoskune <mcoskune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 11:47:57 by mcoskune          #+#    #+#             */
/*   Updated: 2024/11/04 16:59:07 by mcoskune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// ./philo [#philo] [t_die] [t_eat] [t_sleep] [num_eat(optional)]

// valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./philo 10 10 10 10

// valgrind --tool=helgrind --log-file=valgrind-out.txt ./philo 10 10 10 10

void	mini_waits(t_philo *philo, int delta_t)
{
	long long	t_end_wait;

	t_end_wait = get_current_time(philo) + delta_t;
	while (get_current_time(philo) < t_end_wait)
	{
		usleep(10);
	}
}

static void	join_philosophers(t_philo *philo)
{
	int	i;

	i = 0;
	// print_msg(philo, -1, get_current_time(philo), "MAIN WAITING TO JOIN PHILOS");
	// print_msg(philo, -1, philo->num_of_philo, "NUM OF PHILO FROM MAIN");
	while (i < philo->num_of_philo)
	{
		pthread_join(philo->profs[i]->prof, NULL);
		i++;
		print_msg(philo, -1, get_current_time(philo), "MAIN JOINED A PHILO");
	}
	printf("JOINED PHILOS\n");
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
	philo.t_start = get_current_time(&philo);
	pthread_mutex_unlock(&philo.start_sig_mutex);
	join_philosophers(&philo);
	pthread_join(philo.judge_dredd, NULL);
	printf("JOINED DREDD\n");
	exit_cleanup(&philo, 0, 0);
	return (EXIT_SUCCESS);
}
