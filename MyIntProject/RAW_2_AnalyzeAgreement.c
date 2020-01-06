#include<stdio.h>
#include<string.h>

#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/ether.h>
#include<stdlib.h>

#include <stdio.h>

#include <netinet/ip.h>

#include <netinet/udp.h>

#include <sys/socket.h>

#include <netpacket/packet.h>

#include <net/if_arp.h>

#include <arpa/inet.h>

#include <netinet/ether.h>

#include <sys/ioctl.h>

#include <unistd.h>

#include <netpacket/packet.h>

#include <net/if.h>

#include <string.h>

#include <pthread.h>

#include <net/ethernet.h>

#include <ifaddrs.h>
int main()
{
	int fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	printf("fd = %d\n",fd);
	struct sockaddr_ll sll;
	struct ifreq ethreq;
	while(1){
		unsigned char buf[1500] = "";
		int len = recvfrom(fd,buf,sizeof(buf),0,NULL,NULL);
		char src_mac[18] = "";
		char dst_mac[18] = "";
		char ens33_mac[6] = {0x00,0x0C,0x29,0x75,0x13,0x7B};
		char ens39_mac[6] = {0x00,0x0C,0x29,0x75,0x13,0x85};
		
		char pc_mac[6] = {0x54,0xee,0x75,0x95,0x8b,0x6f};
		char Arm_mac[6]= {0x00,0x53,0x50,0x00,0x2C,0x63};
		unsigned short type = 0;
		
		type = ntohs(*(unsigned short *)(buf+12));
		//连PC
		if(memcmp(buf,ens33_mac,6) == 0  )
		{
			memcpy(buf,ens39_mac,6);
			memcpy(buf+6,Arm_mac,6);
			
			
			strncpy(ethreq.ifr_name,"ens39",IFNAMSIZ);
			ioctl(fd,SIOCGIFINDEX,&ethreq);
		
		bzero(&sll,sizeof(sll));
		sll.sll_ifindex = ethreq.ifr_ifindex;
			
			printf("ens33\n");
			printf("%d\n",len);
		}
		else if(memcmp(buf,ens39_mac,6) == 0)
		{
			memcpy(buf,ens33_mac,6);
			memcpy(buf+6,pc_mac,6);
			
			
		strncpy(ethreq.ifr_name,"ens33",IFNAMSIZ);
		ioctl(fd,SIOCGIFINDEX,&ethreq);
		
		bzero(&sll,sizeof(sll));
		sll.sll_ifindex = ethreq.ifr_ifindex;
		printf("ens39\n");
printf("%d\n",len);
		}
		sendto(fd,buf,len,0,(struct sockaddr *)&sll,sizeof(sll));
		
	}
	close(fd);
return 0;
}