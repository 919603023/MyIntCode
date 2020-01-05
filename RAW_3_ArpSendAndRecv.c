#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/ether.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<netpacket/packet.h>
#include<net/if.h>
#include<string.h>
#include<pthread.h>
void *Send(void * arg)
{
while(1)
{	
	int fd  =  *(int *)arg;
		unsigned char recv_buf[1500] = "";
		recvfrom(fd,recv_buf,sizeof(recv_buf),0,NULL,NULL);
		if(ntohs(*(unsigned short *)(recv_buf+12)) == 0x0806)
		{
			if(ntohs(*(unsigned short *)(recv_buf+14+6)) == 2)
			{
				char mac[18] = "";
				sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x",recv_buf[6],recv_buf[7],recv_buf[8],recv_buf[9],recv_buf[10],recv_buf[11]);
				char ip[16] = "";
				sprintf(ip,"%d.%d.%d.%d",recv_buf[28],recv_buf[29],recv_buf[30],recv_buf[31]);
				printf("%s --> %s\n",ip,mac);
			}
		}
}
}
int main()
{
unsigned char dst_mac[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char msg[1500]={0xff,0xff,0xff,0xff,0xff,0xff,
                             0x00,0x0c,0x29,0x17,0x32,0x77,
                             0x08,0x06,
                             0x00,0x01,/*硬件类型*/   
                             0x08,0x00,/*协议类型*/
                             6,/*硬件地址长度*/
                             4,/*协议地址长度*/
                             0x00,0x01,/*ARP请求*/
                             0x00,0x0c,0x29,0x17,0x32,0x77,
                             10,0,121,166,/*源IP*/
                             0x00,0x00,0x00,0x00,0x00,0x00,/*目的mac*/
                             10,0,121,217/*目的IP*/
                             
    };
	int fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	int len = 42;
	struct ifreq ethreq;
	strncpy(ethreq.ifr_name,"ens33",IFNAMSIZ);
	ioctl(fd,SIOCGIFINDEX,&ethreq);
	struct sockaddr_ll sll;
	bzero(&sll,sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
	pthread_t tid;
	pthread_create(&tid,NULL,Send,(void *)&fd);
	pthread_detach(tid);
	int i = 1;
	while(i < 255 )
	{
		msg[41] = i ;
		sendto(fd,msg,len,0,(struct sockaddr *)&sll,sizeof(sll));
		i++;
	}
	sleep(1);
	close(fd);	
	return 0;
}	
