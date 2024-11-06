/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcoskune <mcoskune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 11:48:23 by mcoskune          #+#    #+#             */
/*   Updated: 2024/11/06 16:00:09 by mcoskune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <limits.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>

typedef struct s_prof
{
	pthread_t		prof;
	int				prof_id;
	int				num_eaten;
	long long		last_meal;
	pthread_mutex_t	meal_mutex;
	int				fork[2];
	struct s_philo	*philo_main;
}	t_prof;

typedef struct s_philo
{
	int				num_of_philo;
	long long		t_start;
	int				dt_die;
	int				dt_eat;
	int				dt_think;
	int				dt_sleep;
	int				min_eat_req;
	volatile bool	start_signal;
	volatile bool	kill_signal;
	pthread_t		judge_dredd;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	start_sig_mutex;
	pthread_mutex_t	kill_sig_mutex;
	pthread_mutex_t	*fork_mutex;
	struct s_prof	**profs;
}	t_philo;

typedef enum s_status
{
	DEAD,
	EATING,
	THINKING,
	SLEEPING,
	GOT_FORK_LEFT,
	GOT_FORK_RIGHT,
}	t_status;

/*------- VALIDATE & INITIALIZE -------*/
int			input_validate(int ac, char **av);
int			clean_initialize(t_philo *philo, int ac, char **av);

/*------- SIMULATION -------*/
void		*philo_routine(void *argv);
void		*judge_routine(void *argv);
void		mini_waits(t_philo *philo, int delta_t);
void		eat_routine(t_philo *philo, t_prof *prof);
void		think_routine(t_philo *philo, t_prof *prof);
void		sleep_routine(t_philo *philo, t_prof *prof);

/*------- UTILITIY_FUNCTIONS -------*/
void		msg(const char *message);
int			ft_atoi(const char *str);
long		ft_atol(const char *str);
long long	get_cur_time(t_philo *philo);
void		print_msg(t_philo *philo, int prof_id, long long time, char *str);
bool		check_death(t_philo *philo);

/*------- CLEANUP -------*/
void		exit_cleanup(t_philo *philo, int stage, int iteration);

#endif