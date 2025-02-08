/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:42:55 by anuketay          #+#    #+#             */
/*   Updated: 2025/02/08 18:47:03 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_error_and_exit(char *s)
{
	ft_putstr_fd("Error", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\n", 2);
	pthread_exit(NULL);
}

size_t	get_current_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	print_state(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->tablo->display);
	if (!philo->tablo->is_dead && !philo->tablo->full)
	{
		printf("%zu %d %s\n", get_current_time() - philo->tablo->start_time,
			philo->id, msg);
	}
	pthread_mutex_unlock(&philo->tablo->display);
}

int	convert_to_int(const char *str)
{
	long	result;
	int		i;

	i = 0;
	result = 0;
	while (str[i] >= '0' && str[i] <= '9')
		result = (str[i++] - '0') + (result * 10);
	if (i > 12 || result > 2147483647)
		print_error_and_exit("Invalid argument Integer overflow");
	return ((int)result);
}

void	cleanup_and_exit(t_info *info, pthread_t *id)
{
	int	i;

	i = -1;
	while (++i < info->num_phi)
		pthread_join(id[i], NULL);
	i = -1;
	while (++i < info->num_phi)
		pthread_mutex_destroy(&info->philos[i].fork);
	pthread_mutex_destroy(&info->display);
	pthread_mutex_destroy(&info->check);
	free(info->philos);
	free(id);
}
