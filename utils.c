/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcoskune <mcoskune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 00:40:50 by mcoskune          #+#    #+#             */
/*   Updated: 2024/11/06 16:13:01 by mcoskune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_msg(t_philo *philo, int prof_id, long long time, char *str)
{
	pthread_mutex_lock(&philo->print_mutex);
	pthread_mutex_lock(&philo->kill_sig_mutex);
	if (philo->kill_signal == false || prof_id == -1)
		printf("%lld %d %s\n", time, prof_id, str);
	pthread_mutex_unlock(&philo->kill_sig_mutex);
	pthread_mutex_unlock(&philo->print_mutex);
}

long long	get_cur_time(t_philo *philo)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec) * 1000 + (time.tv_usec / 1000) - philo->t_start);
}

long	ft_atol(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str [i] == '+')
		i++;
	else if (str [i] == '-')
	{
		sign *= -1;
		i++;
	}
	while (str[i] <= '9' && str[i] >= '0')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	int		result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	if (str [i] == '+')
		i++;
	else if (str [i] == '-')
	{
		sign *= -1;
		i++;
	}
	while (str[i] <= '9' && str[i] >= '0')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

void	msg(const char *message)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	if (message == NULL)
		return ;
	while (message[i] != '\0')
	{
		count = write(2, &message[i], 1);
		i++;
	}
	(void) count;
}
