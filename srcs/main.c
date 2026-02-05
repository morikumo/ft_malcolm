#include "ft_malcolm.h"

t_env g_env;

static void sigint_handler(int sig)
{
    (void)sig;
    g_env.running = 0;
}

int main(int argc, char **argv)
{
    struct in_addr src_ip, target_ip;
    unsigned char src_mac[6], target_mac[6];
    struct timeval tv;

    if (argc != 5)
    {
        printf("Usage: ./ft_malcolm <src_ip> <src_mac> <target_ip> <target_mac>\n");
        return (1);
    }

    if (!parse_ip(argv[1], &src_ip))
        return (printf("ft_malcolm: unknown host or invalid IP address: (%s).\n", argv[1]), 1);
    if (!parse_mac(argv[2], src_mac))
        return (printf("ft_malcolm: invalid mac address: (%s)\n", argv[2]), 1);
    if (!parse_ip(argv[3], &target_ip))
        return (printf("ft_malcolm: unknown host or invalid IP address: (%s).\n", argv[3]), 1);
    if (!parse_mac(argv[4], target_mac))
        return (printf("ft_malcolm: invalid mac address: (%s)\n", argv[4]), 1);

    g_env.sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (g_env.sockfd < 0)
        return (perror("socket"), 1);

    /* ⬇ AJOUT IMPORTANT : timeout recvfrom */
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(g_env.sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    g_env.running = 1;
    signal(SIGINT, sigint_handler);

    find_interface(g_env.sockfd);
    listen_and_spoof(g_env.sockfd, src_ip, src_mac, target_ip, target_mac);

    close(g_env.sockfd);
    return (0);
}
