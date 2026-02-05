#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <alpha-linux-gnu/asm/socket.h>
#include <ifaddrs.h>


#define ARP_REQUEST 1
#define ARP_REPLY 2

typedef struct s_env
{
	int sockfd;
	int running;
}	t_env;

extern t_env g_env;

int parse_ip(const char *str, struct in_addr *addr);
int parse_mac(const char *str, unsigned char *mac);
int find_interface(int sockfd);
void listen_and_spoof(
	int sockfd,
	struct in_addr src_ip,
	unsigned char *src_mac,
	struct in_addr target_ip,
	unsigned char *target_mac
);

#endif
