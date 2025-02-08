/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:42:59 by anuketay          #+#    #+#             */
/*   Updated: 2025/02/08 19:06:34 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	validate_arguments(int argc, char *argv[])
{
	int	arg_index;
	int	char_index;

	if (argc < 5 || argc > 6)
		print_error_and_exit("Invalid number of arguments");
	char_index = 0;
	while (argv[0][char_index])
		char_index++;
	arg_index = 1;
	while (argv[arg_index])
	{
		char_index = 0;
		while (argv[arg_index][char_index])
		{
			if (!(argv[arg_index][char_index] >= '0'
				&& argv[arg_index][char_index] <= '9'))
				print_error_and_exit("Invalid argument");
			char_index++;
		}
		arg_index++;
	}
}

static void	check_philosopher_death(t_info *info, int index)
{
	if (get_current_time()
		- info->philos[index].last_eat > (size_t)info->die_time)
	{
		print_state(&info->philos[index], "died 💀");
		info->is_dead = 1;
	}
}

void	monitor_philosophers(t_info *info)
{
	int	index;

	while (!info->full)
	{
		index = -1;
		while (!info->is_dead && ++index < info->num_phi)
		{
			pthread_mutex_lock(&info->check);
			check_philosopher_death(info, index);
			pthread_mutex_unlock(&info->check);
			usleep(100);
		}
		if (info->is_dead)
			break ;
		index = 0;
		while (info->max_meals != -1 && index < info->num_phi
			&& info->philos[index].meals >= info->max_meals)
			index++;
		if (index == info->num_phi)
			info->full = 1;
	}
}
