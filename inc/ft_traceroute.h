#ifndef FT_TRACEROUTE
# define FT_TRACEROUTE

#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <math.h>

#define PACKET_SIZE 64
#define IP_HDR_SIZE 20
#define ICMP_HDR_SIZE 8

typedef struct  s_ipv4_header{
    unsigned int    ihl:4;
    uint8_t         tos;
    uint16_t        tot_len;
    uint16_t        id;
    uint16_t        frag_off;
    uint8_t         ttl;
    uint8_t         protocol;
    uint16_t        check;
    uint32_t        saddr;
    uint32_t        daddr;
}               t_ipv4_header;

typedef struct s_icmp_header {
    uint8_t     type;
    uint8_t     code;
    uint16_t    checksum;
    union {
        struct {
            uint16_t id;
            uint16_t sequence;
        } echo;
        uint32_t gateway;
        struct {
            uint16_t __glibc_reserved;
            uint16_t mtu;
        } frag;
    } un;
}               t_icmp_header;

typedef struct s_all {
    t_ipv4_header   ip;
    t_icmp_header   icmp_send;
    t_icmp_header   icmp_receive;
    char            hostname[16];
    char            *addr;
    int             packet_sent;
    int             packet_receive;
    double          min;
    double          max;
    double          avg;
    double          data[100000];
    long int        c_flag;
    int             sockfd;
    int             option_n;
    int             option_m;
    int             option_f;
}               t_all;

char            *delete_space(char *chaine);
int		        ft_strncmp(const char *s1, const char *s2, size_t n);
int		        ft_strlen(char *s);
long int	    ft_atoi(char *nptr);
int             ft_isdigit(char *str);
void            get_name_adress(char *ip_address);
void            get_the_adresse(char *hostname);
void            no_arg();
void            traceroute_help();
void            verification_arg(int argc, char **argv);
int             recv_packet(int ttl, int write_ip, char *ip_rec,  int i, int finish, struct timeval start_time);
unsigned short  checksum(unsigned short *buf, int len);
void            ft_traceroute(t_icmp_header icmp_header, struct sockaddr_in dest_addr);



extern t_all    g_all;

#endif