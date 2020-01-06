#include<stdio.h>
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
	while(1){
		unsigned char buf[1500] = "";
		int len = recvfrom(fd,buf,sizeof(buf),0,NULL,NULL);
		char src_mac[18] = "";
		char dst_mac[18] = "";
		unsigned short type = 0;
		sprintf(dst_mac,"%02x:%02x:%02x:%02x:%02x:%02x",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
        struct ifreq ethreq;
        struct sockaddr_ll sll;
        if(buf[0] == 0x00 && buf[1] == 0x0C && buf[2] == 0x29 && buf[3] == 0x75 && buf[4] == 0x13 && buf[5] == 0x7b)
        {
            buf[0] = 0x00;
            buf[1] = 0x0C;
            buf[2] = 0x29;
            buf[3] = 0x75;
            buf[4] = 0x13;
            buf[5] = 0xA3;

            buf[6] = 0x00;
            buf[7] = 0x53;
            buf[8] = 0x50;
            buf[9] = 0x00;
            buf[10] = 0x3A;
            buf[11] = 0xC7;
    
	strncpy(ethreq.ifr_name,"ens38",IFNAMSIZ);
	ioctl(fd,SIOCGIFINDEX,&ethreq);
	
	bzero(&sll,sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
        }
      else  if(buf[6-6] == 0x00&&
            buf[7-6] == 0x53&&
            buf[8-6] == 0x50&&
            buf[9-6] == 0x00&&
            buf[10-6] == 0x3A&&
            buf[11-6] == 0xC7)
            {
            buf[0+6] = 0x00;
            buf[1+6] = 0x0C;
            buf[2+6] = 0x29;
            buf[3+6] = 0x75;
            buf[4+6] = 0x13;
            buf[5+6] = 0xA3;
            strncpy(ethreq.ifr_name,"ens33",IFNAMSIZ);
	ioctl(fd,SIOCGIFINDEX,&ethreq);
	
	bzero(&sll,sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
            }
		sprintf(src_mac,"%02x:%02x:%02x:%02x:%02x:%02x",buf[0+6],buf[1+6],buf[2+6],buf[3+6],buf[4+6],buf[5+6]);
		type = ntohs(*(unsigned short *)(buf+12));
		printf("协议类型：type = %#x\n",type);
		printf("%s >> %s\n",src_mac,dst_mac);
    sendto(fd,buf,len,0,(struct sockaddr *)&sll,sizeof(sll));
    }
	close(fd);
return 0;
}