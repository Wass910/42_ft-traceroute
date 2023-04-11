#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <unistd.h>
#include "ft_traceroute.h"
#define PACKET_SIZE 64
#define IP_HDR_SIZE 20
#define ICMP_HDR_SIZE 8

t_all g_all = {{0}, {0}, {0}, NULL, NULL, -1, 0, 0, 0, 0, {0}, -5};

void no_arg()
{
    printf("Usage:  ft_traceroute [OPTION...] HOST\n");
    printf("Print the route packets trace to network host.\n");
    printf("\n  --help\t\tgive this help list\n");
    exit(0);
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

int main(int argc, char **argv) {
    if (argc > 3 || argc < 2)
        no_arg();
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    

    // Remplissage du header ICMP
    t_icmp_header icmp_header;
    icmp_header.type = ICMP_ECHO;
    icmp_header.code = 0;
    icmp_header.checksum = 0;
    icmp_header.un.echo.id = htons(getpid());
    icmp_header.un.echo.sequence = 0;
    icmp_header.checksum = checksum((unsigned short*)&icmp_header, ICMP_HDR_SIZE);
    // Envoi du paquet ICMP à Google
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "8.8.8.8", &(dest_addr.sin_addr));
    


    // Attente de la réponse de Google
    struct sockaddr_in recv_addr;
    socklen_t addr_len = sizeof(recv_addr);
    struct msghdr recv_msg;
    memset(&recv_msg, 0, sizeof(recv_msg));
    struct iovec iov[1];
    char recv_buffer[PACKET_SIZE];
    iov[0].iov_base = recv_buffer;
    iov[0].iov_len = PACKET_SIZE;
    recv_msg.msg_name = &recv_addr;
    recv_msg.msg_namelen = addr_len;
    recv_msg.msg_iov = iov;
    recv_msg.msg_iovlen = 1;
    int ttl = 1;
    while (1)
    {
        printf("\nttl = %d\n", ttl);
        if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
            perror("setsockopt");
            exit(1);
        }
        ssize_t bytes_sent = sendto(sockfd, &icmp_header, ICMP_HDR_SIZE, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
        if (bytes_sent < 0) {
            perror("sendto");
            exit(1);
        }
        ssize_t bytes_received = recvmsg(sockfd, &recv_msg, 0);
        if (bytes_received < 0) {
            perror("recvmsg");
            exit(1);
        }
        else
            printf("ok\n");
        g_all.ip  = *(t_ipv4_header*)recv_buffer;
        g_all.icmp_receive = *(t_icmp_header*)(recv_buffer + (g_all.ip.ihl * 4));
        if (g_all.icmp_receive.type == ICMP_ECHOREPLY){
            printf("Ping successful! et ttl = %d\n", g_all.ip.ttl);
            return 0;
        }
        else if (g_all.icmp_receive.type == ICMP_TIMXCEED){
            printf("time exceeded ! et ttl = %d\n", g_all.ip.ttl);
            printf("%d   ip = %s\n", ttl, inet_ntoa(recv_addr.sin_addr));
        }
        else{ 
            printf("Ping failed!\n");
            return 0;
        }
        printf("\n ");
        ttl++;
    }
    return 0;
}

