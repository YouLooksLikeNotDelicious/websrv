#ifndef __ARP_H__
#define __ARP_H__

int init_arp_table_handle();

void release_arp_table_handle(int handle);

char * get_mac(int sock, const char *ip, const char *dev, char *buf, int len);

#endif
