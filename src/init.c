#include "philo.h"

void	initialize_threads(t_info *info, pthread_t **id)
{
    int	index;

    index = -1;
    while (++index < info->num_phi)
    {
        if (pthread_create(&id[0][index], NULL, &routine, &info->philos[index]))
        {
            free(info->philos);
            free(*id);
            print_error_and_exit("Failed to create thread");
        }
        pthread_mutex_lock(&(info->check));
        info->philos[index].last_eat = info->start_time;
        pthread_mutex_unlock(&info->check);
    }
}

void	initialize_arguments(char *argv[], t_info *info)
{
	info->num_phi = convert_to_int(argv[1]);
	info->die_time = convert_to_int(argv[2]);
	info->eat_time = convert_to_int(argv[3]);
	info->sleep_time = convert_to_int(argv[4]);
	info->max_meals = -1;
	if (argv[5])
		info->max_meals = convert_to_int(argv[5]);
	info->is_dead = 0;
	info->full = 0;
}

void	initialize_philosophers(t_info *info)
{
	int	index;

	info->philos = (t_philo *)malloc(info->num_phi * sizeof(t_philo));
	if (!info->philos)
		print_error_and_exit("Failed to allocate memory");
	index = -1;
	while (++index < info->num_phi)
	{
		info->philos[index].id = index + 1;
		info->philos[index].tablo = info;
		info->philos[index].meals = 0;
		if (index + 1 == info->num_phi)
			info->philos[index].next = & info->philos[0];
		else
			info->philos[index].next = &info->philos[index + 1];
		if (index == 0)
			info->philos[index].prev = &info->philos[info->num_phi - 1];
		else
			info->philos[index].prev = &info->philos[index - 1];
		pthread_mutex_init(&info->philos[index].fork, NULL);
	}
	pthread_mutex_init(&info->display, NULL);
	pthread_mutex_init(&info->check, NULL);
}
