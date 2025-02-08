#include "philo.h"

void	philosopher_eat(t_philo *philo, t_info *info)
{
    pthread_mutex_lock(&philo->fork);
    print_state(philo, "has taken a fork");
    if (info->num_phi == 1)
    {
        usleep(info->die_time * 1000);
        print_state(philo, "died");
        pthread_mutex_unlock(&philo->fork);
        info->is_dead = 1;
        return ;
    }
    pthread_mutex_lock(&philo->next->fork);
    print_state(philo, "has taken a fork");
    pthread_mutex_lock(&info->check);
    philo->meals++;
    print_state(philo, "is eating");
    philo->last_eat = get_current_time();
    pthread_mutex_unlock(&info->check);
    usleep(info->eat_time * 1000);
    pthread_mutex_unlock(&philo->fork);
    pthread_mutex_unlock(&philo->next->fork);
}

void	*routine(void *arg)
{
    t_philo	*philo;
    t_info	*info;

    philo = (t_philo *)arg;
    info = philo->tablo;
    if (philo->id % 2 == 0)
        usleep(1000);
    while (!info->is_dead && !info->full)
    {
        philosopher_eat(philo, philo->tablo);
        if (info->is_dead)
            break;
        print_state(philo, "is sleeping");
        usleep(info->sleep_time * 1000);
        if (info->is_dead)
            break;
        print_state(philo, "is thinking");
    }
    return (NULL);
}

int main(int argc, char *argv[])
{
    t_info info;
    pthread_t *id;

    validate_arguments(argc, argv);
    initialize_arguments(argv, &info);
    initialize_philosophers(&info);
    id = (pthread_t *)malloc(info.num_phi * sizeof(pthread_t));
    info.start_time = get_current_time();
    initialize_threads(&info, &id);
    monitor_philosophers(&info);
    cleanup_and_exit(&info, id);
    return (0);
}
