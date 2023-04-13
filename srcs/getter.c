#include "../inc/ft_traceroute.h"

void    get_the_adresse(char *hostname) 
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    int status = getaddrinfo(hostname, NULL, &hints, &res);
    if (status != 0) {
        printf("traceroute: cannot resolve %s: Unknown host\n", hostname);
        exit(1);
    }
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)res->ai_addr;
    inet_ntop(AF_INET, &(ipv4->sin_addr), g_all.hostname, INET_ADDRSTRLEN);
    freeaddrinfo(res);
    return ;
}

void    get_name_adress(char *ip_address)
{
    struct in_addr addr;
    struct hostent *host;

    if (inet_aton(ip_address, &addr) == 0)
    {
        printf("Adresse IP invalide : %s\n", ip_address);
        return ;
    }

    if (g_all.option_n == 0)
    {
        if ((host = gethostbyaddr(&addr, sizeof(addr), AF_INET)) == NULL)
        {
            printf("%s ", ip_address);
            return ;
        }
        printf("%s (%s) ", host->h_name, ip_address);
    }
    else
        printf("%s ", ip_address);
    return ;
}