#include "../inc/ft_traceroute.h"

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n <= 0)
		return (0);
	while (n > 1 && (*s1 != '\0' && *s2 != '\0') && *s1 == *s2)
	{
		s1++;
		s2++;
		n--;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int		ft_isdigit(char *str)
{
    int i = 0;
	int plus = 0;

    while (str[i])
    {
        if ((str[i] < '0' || str[i] > '9') && str[i] != '+')
            return (1);
		if (str[i] == '+')
			plus++;
        i++;
    }
	if (plus > 1)
		return 1;
	else if (plus == 1)
	{
		if (str[0] == '+')
			return 0;
		else 
			return 1;
	}
	return (0);
}

int		ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

long int		ft_atoi(char *nptr)
{
	int i;
	int sign;
	long int nb;

	sign = 1;
	nb = 0;
	i = 0;
	while (nptr[i] == '\n' || nptr[i] == '\t' || nptr[i] == '\v' ||
		nptr[i] == '\f' || nptr[i] == '\r' || nptr[i] == ' ')
	{
		i++;
	}
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = sign * -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nb = nb * 10 + nptr[i] - '0';
		i++;
	}
	return (nb * sign);
}

char *delete_space(char *chaine) 
{
    int i, j = 0;

    for (i = 0; chaine[i] != '\0'; i++) {
        if (chaine[i] != ' ') {
            chaine[j] = chaine[i];
            j++;
        }
    }
    chaine[j] = '\0';
    return( chaine );
}

unsigned short checksum(unsigned short *buf, int len) {
    unsigned long sum = 0;
    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }
    if (len == 1) {
        sum += *(unsigned char*)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}
