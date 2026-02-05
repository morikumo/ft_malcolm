#include "ft_malcolm.h"

int parse_ip(const char *str, struct in_addr *addr)
{
        return (inet_pton(AF_INET, str, addr) == 1);
}

int parse_mac(const char *str, unsigned char *mac)
{
        return (sscanf(str, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                &mac[0], &mac[1], &mac[2],
                &mac[3], &mac[4], &mac[5]) == 6);
}

int find_interface(int sockfd)
{
        struct ifaddrs *ifaddr, *ifa;

        getifaddrs(&ifaddr);
        for (ifa = ifaddr; ifa; ifa = ifa->ifa_next)
        {
                if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_PACKET)
                        continue;

                if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE,
                        ifa->ifa_name, strlen(ifa->ifa_name)) == 0)
                {
                        printf("Found available interface: %s\n", ifa->ifa_name);
                        break;
                }
        }
        freeifaddrs(ifaddr);
        return (0);
}