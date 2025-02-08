# Dining Philosophers Problem

## Introduction

The Dining Philosophers Problem is a classic synchronization problem in computer science. It involves a certain number of philosophers sitting around a table, each with a fork on their left and right. The philosophers alternate between thinking and eating. However, a philosopher can only eat if they have both the left and right forks. The challenge is to design a solution that ensures no philosopher starves and avoids deadlock.

## How to Download and Compile

To download and compile the project, follow these steps:

1. Clone the repository:
   ```sh
   git clone https://github.com/nuketayev/Threads.Mutexes-Dining_philosophers_problem.git
   cd Threads.Mutexes-Dining_philosophers_problem
   ```

2. Compile the project using the provided Makefile:
   ```sh
   make
   ```

## Usage

To run the program, use the following command:
```sh
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
```

Example:
```sh
./philo 5 800 200 200 7
```

## Detailed Documentation

### Code Structure

The project consists of several files:

- `include/philo.h`: Header file containing structure definitions and function prototypes.
- `libft/ft_putstr_fd.c`: Utility function to print a string to a file descriptor.
- `Makefile`: Makefile to compile the project.
- `src/init.c`: Functions to initialize philosophers and threads.
- `src/main.c`: Main function and philosopher routine.
- `src/utilities.c`: Utility functions for time management and state printing.
- `src/validate.c`: Functions to validate arguments and monitor philosophers.

### Function Explanations

#### `include/philo.h`

This header file contains the structure definitions and function prototypes used throughout the project.

#### `libft/ft_putstr_fd.c`

This file contains the `ft_putstr_fd` function, which prints a string to a file descriptor.

```c
void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}
```

#### `src/init.c`

This file contains functions to initialize philosophers and threads.

- `initialize_threads`: Creates threads for each philosopher.
- `initialize_arguments`: Initializes the arguments passed to the program.
- `initialize_philosophers`: Initializes the philosopher structures.

```c
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
```

#### `src/main.c`

This file contains the main function and the philosopher routine.

- `philosopher_eat`: Handles the eating process for a philosopher.
- `routine`: The routine executed by each philosopher thread.
- `main`: The main function that initializes and starts the simulation.

```c
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
        usleep(info->sleep_time * 1000);
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
```

#### `src/utilities.c`

This file contains utility functions for time management and state printing.

- `print_error_and_exit`: Prints an error message and exits the program.
- `get_current_time`: Returns the current time in milliseconds.
- `print_state`: Prints the current state of a philosopher.
- `convert_to_int`: Converts a string to an integer.
- `cleanup_and_exit`: Cleans up resources and exits the program.

```c
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
        printf("%zu %d %s\n", get_current_time() - philo->tablo->start_time, philo->id, msg);
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

void cleanup_and_exit(t_info *info, pthread_t *id)
{
    int i;

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
```

#### `src/validate.c`

This file contains functions to validate arguments and monitor philosophers.

- `validate_arguments`: Validates the arguments passed to the program.
- `monitor_philosophers`: Monitors the state of the philosophers and checks for death or completion.

```c
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
            if (!(argv[arg_index][char_index] >= '0' && argv[arg_index][char_index] <= '9'))
                print_error_and_exit("Invalid argument");
            char_index++;
        }
        arg_index++;
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
            if (get_current_time() - info->philos[index].last_eat > (size_t)info->die_time)
            {
                print_state(&info->philos[index], "died 💀");
                info->is_dead = 1;
            }
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
```

## Test Scenarios

To validate the project, the following test scenarios should be executed:

1. Test with 1 800 200 200, the philosopher should not eat and should die!
   ```sh
   ./philo 1 800 200 200
   ```

2. Test with 5 800 200 200, no one should die!
   ```sh
   ./philo 5 800 200 200
   ```

3. Test with 5 800 200 200 7, no one should die and the simulation should stop when all the philosophers have eaten at least 7 times each.
   ```sh
   ./philo 5 800 200 200 7
   ```

4. Test with 4 410 200 200, no one should die!
   ```sh
   ./philo 4 410 200 200
   ```

5. Test with 4 310 200 100, a philosopher should die!
   ```sh
   ./philo 4 310 200 100
   ```

6. Test with 2 philosophers and check the different times (a death delayed by more than 10 ms is unacceptable).
   ```sh
   ./philo 2 800 200 200
   ```

Note: Do not test with more than 200 philosophers or with `time_to_die`, `time_to_eat`, or `time_to_sleep` under 60 ms.
