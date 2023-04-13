#include "../inc/ft_traceroute.h"

void no_arg()
{
    printf("Usage:  ft_traceroute HOST [OPTION...]\n");
    exit(0);
}

void    traceroute_help()
{
    printf("Usage:\n");
    printf("  ft_traceroute [ -fmn ]\n");
    printf("Options:\n");
    printf("  -f first_ttl               Start from the first_ttl hop (instead from 1)\n");
    printf("  -m max_ttl                 Set the max number of hops (max TTL to be reached). Default is 30\n");
    printf("  -n                         Do not resolve IP addresses to their domain names\n");
    exit(0);
}

void    verification_arg(int argc, char **argv)
{
    int i = 0;
    while (i < argc)
    {
        if (ft_strncmp(delete_space(argv[i]), "--help", ft_strlen("--help")) == 0 && ft_strlen(argv[i]) == ft_strlen("--help"))
            traceroute_help();
        i++;
    }
    i = 2;
    while (i < argc)
    {
        if (ft_strncmp(delete_space(argv[i]), "-n", ft_strlen("-n")) == 0 && ft_strlen(argv[i]) == ft_strlen("-n"))
            g_all.option_n = 1;
        else if (ft_strncmp(delete_space(argv[i]), "-m", ft_strlen("-m")) == 0 && ft_strlen(argv[i]) == ft_strlen("-m"))
        {
            if (argv[i + 1] && ft_isdigit(argv[i + 1]) == 0){
                g_all.option_m = ft_atoi(argv[i + 1]);
                if (g_all.option_m == 0){
                    printf("first hop out of range\n");
                    exit(EXIT_FAILURE);
                }
            }
            else{
                printf("Cannot handle `-m' option with bad arg\n");
                exit(EXIT_FAILURE);
            }
            i++;
        }
        else if (ft_strncmp(delete_space(argv[i]), "-f", ft_strlen("-f")) == 0 && ft_strlen(argv[i]) == ft_strlen("-f"))
        {
            if (argv[i + 1] && ft_isdigit(argv[i + 1]) == 0){
                g_all.option_f = ft_atoi(argv[i + 1]);
                if (g_all.option_f == 0){
                    printf("first hop out of range\n");
                    exit(EXIT_FAILURE);
                }
            }
            else{
                printf("Cannot handle `-f' option with bad arg\n");
                exit(EXIT_FAILURE);
            }
            i++;
        }
        else
            no_arg();
        i++;
    }
    if (g_all.option_f > g_all.option_m)
    {
        printf("first hop out of range\n");
        exit(EXIT_FAILURE);
    }
    return ;
}
