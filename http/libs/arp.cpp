#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/ioctl.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "arp.h"

char * get_mac(int sock, const char *ip, const char *dev, char *buf, int len)
{
    struct arpreq arp;
    struct in_addr in;
    memset(&arp, 0, sizeof(struct arpreq));
    struct sockaddr_in *sin = (struct sockaddr_in *)&arp.arp_pa;
    sin->sin_family = AF_INET;
    in.s_addr = inet_addr(ip);
    memcpy(&sin->sin_addr, (char *)&in, sizeof(struct in_addr));
    strcpy(arp.arp_dev, dev);
    int rc = ioctl(sock, SIOCGARP, &arp);
    if ( rc < 0 ) {
        return (char *)NULL;
    }
    else {
        unsigned char *hw = (unsigned char *)arp.arp_ha.sa_data;
        snprintf(buf, len, "%02x:%02x:%02x:%02x:%02x:%02x", hw[0], hw[1], hw[2], hw[3], hw[4], hw[5]);
    }
    return buf;
}

int init_arp_table_handle()
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    return sock;
}

void release_arp_table_handle(int handle)
{
    close(handle);
}

#ifdef __TEST_ARP_TEST_
int main()
{
    char mac[32];
    int handle = init_arp_table_handle();
    get_mac(handle, "192.168.7.100", "eth0", mac, sizeof(mac));
    release_arp_table_handle(handle);
    printf("%s\n", mac);
    return 0;
}
#endif
