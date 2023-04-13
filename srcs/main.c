#include "../inc/ft_traceroute.h"

t_all g_all = {{0}, {0}, {0}, "", NULL, -1, 0, 0, 0, 0, {0}, -5, 0, 0, 30, 1};

int main(int argc, char **argv) {
    if (getuid() != 0) {
        printf("Run ft_traceroute as sudo.\n");
        exit(1);
    }
    if (argc < 2 )
        no_arg();
    verification_arg(argc, argv);

    g_all.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (g_all.sockfd < 0) {
        perror("socket");
        exit(1);
    }

    g_all.addr = argv[1];
    get_the_adresse(argv[1]);
    t_icmp_header icmp_header;
    icmp_header.type = ICMP_ECHO;
    icmp_header.code = 0;
    icmp_header.checksum = 0;
    icmp_header.un.echo.id = htons(getpid());
    icmp_header.un.echo.sequence = 0;
    icmp_header.checksum = checksum((unsigned short*)&icmp_header, ICMP_HDR_SIZE);

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    int test = inet_pton(AF_INET, g_all.hostname, &(dest_addr.sin_addr));
    if (test != 1)
    {
        printf("Please enter an IPV4 address\n");
        return 1;
    }
    ft_traceroute(icmp_header, dest_addr);
    return 0;
}
