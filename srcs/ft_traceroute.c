#include "../inc/ft_traceroute.h"

int    recv_packet(int ttl, int write_ip, char *ip_rec,  int i, int finish, struct timeval start_time)
{
    struct msghdr recv_msg;
    struct sockaddr_in recv_addr;
    socklen_t addr_len = sizeof(recv_addr);
    memset(&recv_msg, 0, sizeof(recv_msg));
    struct iovec iov[1];
    char recv_buffer[PACKET_SIZE];
    iov[0].iov_base = recv_buffer;
    iov[0].iov_len = PACKET_SIZE;
    recv_msg.msg_name = &recv_addr;
    recv_msg.msg_namelen = addr_len;
    recv_msg.msg_iov = iov;
    recv_msg.msg_iovlen = 1;
    struct timeval end_time;

    ssize_t bytes_received = recvmsg(g_all.sockfd, &recv_msg, 0);
    if (bytes_received < 0) {
        perror("recvmsg");
        exit(1);
    }
    g_all.ip  = *(t_ipv4_header*)recv_buffer;
    g_all.icmp_receive = *(t_icmp_header*)(recv_buffer + (g_all.ip.ihl * 4));
    if (ttl >= g_all.option_f)
    {
        if (i == 0){
            //last_ip_rec = (char *)inet_ntoa(recv_addr.sin_addr);
            
            printf("%2d  ", ttl);
        } 
        ip_rec = (char *)inet_ntoa(recv_addr.sin_addr);
        if (ft_strlen(ip_rec) > 5 )
            write_ip++;
        
        if (g_all.icmp_receive.type == ICMP_TIMXCEED){
            if (write_ip == 1 ){
                get_name_adress(ip_rec);
                //last_ip_rec = ip_rec;
            }
            //printf("%s ",  ip_rec);
        }
        else if (g_all.icmp_receive.type ==  ICMP_DEST_UNREACH){
            if (write_ip == 1 ){
                get_name_adress(ip_rec);
            }
        }
        else if (g_all.icmp_receive.type == ICMP_ECHO){
            if (write_ip == 1 ){
                get_name_adress(ip_rec);
                //last_ip_rec = ip_rec;
            }
            //printf("%s ",  ip_rec);
        }
        else if (g_all.icmp_receive.type == ICMP_ECHOREPLY)
        {
            finish = 10;
            if (write_ip == 1 )
                get_name_adress(ip_rec);
            gettimeofday(&end_time, NULL);
            double long time_diff = ((end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec) / 1000.000);
            if (time_diff > 500)
                printf("* ");
            else
                printf(" %.3Lf ms ", time_diff);
            return finish;
        }
        else
        { 
            printf("Ping failed! type = %d\n", g_all.icmp_receive.type);
            exit(EXIT_FAILURE) ;
        }
        gettimeofday(&end_time, NULL);
        double long time_diff = ((end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec) / 1000.000);
        if (time_diff > 500)
            printf("* ");
        else
            printf(" %.3Lf ms ", time_diff);
    }
    return write_ip;
}

void    ft_traceroute(t_icmp_header icmp_header, struct sockaddr_in dest_addr)
{
    struct timeval  start_time;
    char *ip_rec = "";
    //char *last_ip_rec = "";
    int ttl = 1;
    int i = 0;
    int write_ip = 0;
    int finish = 0;
    struct timeval timeout = {1, 0};
    while (1)
    {
        if (ttl == 1)
            printf("traceroute to %s (%s), %d hops max, 60 byte packets\n", g_all.addr, g_all.hostname, g_all.option_m);
        if (setsockopt(g_all.sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
            perror("setsockopt");
            exit(1);
        }
        while (i < 3)
        {        
            gettimeofday(&start_time, NULL);
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(g_all.sockfd, &read_fds);
            ssize_t bytes_sent = sendto(g_all.sockfd, &icmp_header, ICMP_HDR_SIZE, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
            if (bytes_sent < 0) {
                perror("sendto");
                exit(1);
            }
            int select_result = select(g_all.sockfd + 1, &read_fds, NULL, NULL, &timeout);
            if (select_result == -1)
            {
                perror("Erreur lors de l'utilisation de la fonction select()");
                exit(EXIT_FAILURE);
            }
            else if (select_result == 0)
            {
                if (i == 0)
                    printf("%2d  ", ttl);
                printf("* ");    
                timeout.tv_sec = 0;
                timeout.tv_usec = 300000;
            }
            else{
                finish = recv_packet(ttl, write_ip, ip_rec, i, finish,  start_time);
                write_ip++;
            }
            i++;
        }
        if (i == 3){
            i = 0;
            if (ttl >= g_all.option_f)
                printf("\n");
        }
        write_ip = 0;
        ttl++;
        if (finish == 10 || ttl == (g_all.option_m + 1))
            break ;
    }
    return ;
}
