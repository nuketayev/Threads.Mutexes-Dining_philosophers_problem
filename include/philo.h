/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:42:27 by anuketay          #+#    #+#             */
/*   Updated: 2025/02/08 18:42:30 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				meals;
	size_t			last_eat;
	pthread_mutex_t	fork;
	struct s_philo	*next;
	struct s_philo	*prev;
	struct s_info	*tablo;
}					t_philo;

typedef struct s_info
{
	int				num_phi;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				max_meals;
	size_t			start_time;
	t_philo			*philos;
	pthread_mutex_t	display;
	pthread_mutex_t	check;
	int				is_dead;
	int				full;
}					t_info;

void				ft_putstr_fd(char *s, int fd);
void				*routine(void *arg);
void				initialize_philosophers(t_info *info);
void				initialize_threads(t_info *info, pthread_t **id);
void				print_state(t_philo *philo, char *msg);
void				monitor_philosophers(t_info *info);
void				cleanup_and_exit(t_info *info, pthread_t *id);
size_t				get_current_time(void);
void				validate_arguments(int argc, char *argv[]);
void				initialize_arguments(char *argv[], t_info *info);
int					print_error_and_exit(char *s);
int					convert_to_int(const char *str);
void				philosopher_eat(t_philo *philo, t_info *info);

#endif
